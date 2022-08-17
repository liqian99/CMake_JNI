#include "qian_lee_image_library_ImageLibraryJni.h"
#include "image/ImageProcess.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL Java_qian_lee_image_1library_ImageLibraryJni_zoomImg
  (JNIEnv *env, jclass jzoom, jbyteArray input, jint iw, jint ih, jint ichannels,
                        jbyteArray output, jint ow, jint oh){

         jbyte* jinput = (*env)->GetByteArrayElements(env,input,0);
         jbyte* joutput = (*env)->GetByteArrayElements(env,output,0);

         unsigned char* pinput=(unsigned char*)jinput;
         unsigned char* poutput=(unsigned char*)joutput;

         zoomImg(pinput, iw, ih,ichannels, poutput, ow, oh);

         (*env)->ReleaseByteArrayElements(env,input, jinput, 0);
         (*env)->ReleaseByteArrayElements(env,output, joutput, 0);

         return 0;

  }

#ifdef __cplusplus
}
#endif

