package qian.lee.image_library;

public class ImageLibraryJni {
    /**
     * @desc
     * @param
     * @return
     **/
    public static native int zoomImg(byte[] src,int sw,int sh,int schannels,
                                     byte[] dst,int dw,int dh);

    static {
        System.loadLibrary("imagelibrary-lib");
    }
}
