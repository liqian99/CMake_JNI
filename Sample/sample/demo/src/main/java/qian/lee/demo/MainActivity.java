package qian.lee.demo;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;

import java.io.ByteArrayInputStream;
import java.io.Closeable;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;

import qian.lee.image_library.ImageLibraryJni;

public class MainActivity extends AppCompatActivity {
    private static String TAG = MainActivity.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void onClick(View view){
        switch (view.getId()){
            case R.id.bt_test:
                Test();
                break;
            default:
                break;
        }
    }

    private void Test(){
      new Thread(new Runnable() {
          @Override
          public void run() {
              Log.d(TAG,"<MainActivity> *** Start ***");
              Bitmap btSrc = BitmapFactory.decodeFile("/sdcard/src.jpg");
              if(btSrc==null){
                  Log.d(TAG,"<MainActivity> not find image file");
                  return;
              }
              //Bitmap to bytearray
              ByteBuffer bufSrc = ByteBuffer.allocate(btSrc.getByteCount());
              btSrc.copyPixelsToBuffer(bufSrc);
              byte[] byteArraySrc = bufSrc.array();

              //RGB8888 to gray scale,for simplicity, only red
              byte[] byteArraySrcGray =  new byte[btSrc.getByteCount()>>2];
              for(int i=0;i<btSrc.getByteCount();i+=4){
                  byteArraySrcGray[i>>2] = byteArraySrc[i];
              }

              //Zoom
              int sw = btSrc.getWidth(),sh = btSrc.getHeight(),schannels = 1;
              int dw = (sw/2)/4*4,dh = sh/2;
              byte[] byteArrayDstGray = new byte[dw * dh];
              Log.d(TAG,"<MainActivity> sw=" + sw + " sh=" + sh + " byteCount=" + btSrc.getByteCount());
              ImageLibraryJni.zoomImg(byteArraySrcGray,sw,sh,schannels,byteArrayDstGray,dw,dh);

              Log.d(TAG,"<MainActivity> dw=" + dw + " dh=" + dh + " byteCount=" + byteArrayDstGray.length);
              //save
              writeFile(new File("/sdcard/dst.raw"),new ByteArrayInputStream(byteArrayDstGray),byteArrayDstGray.length,false);

              btSrc.recycle();

              Log.d(TAG,"<MainActivity> *** Complete ***");
          }
      }).start();
    }

    public boolean writeFile(File file, InputStream stream, int byteLength, boolean append) {
        OutputStream o = null;
        try {
            o = new FileOutputStream(file, append);
            byte data[] = new byte[byteLength];
            int length = -1;
            while((length = stream.read(data)) != -1) {
                o.write(data, 0, length);
            }
            o.flush();
            return true;
        } catch (FileNotFoundException e) {
            throw new RuntimeException("FileNotFoundException occurred. ", e);
        } catch (IOException e) {
            throw new RuntimeException("IOException occurred. ", e);
        } finally {
            close(o);
            close(stream);
            return false;
        }
    }

    private void close(Closeable closeable) {
        if (closeable != null) {
            try {
                closeable.close();
            } catch (IOException e) {
                throw new RuntimeException("IOException occurred. ", e);
            }
        }
    }
}