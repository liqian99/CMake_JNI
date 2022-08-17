//
// Created by Administrator on 2022/8/17.
//

#include "ImageProcess.h"
#include "../log/log.h"
/**
 * @desc   二次线性插值公式
 *         P0  P2
 *         P1  P3
 *
 *	       pm0=(1-u)*(1-v);
 *		   pm1=v*(1-u);
 *		   pm2=u*(1-v);
 *		   pm3=u*v;
 *
 *         dst = P0*pm0 + P1*pm1 + P2*pm2 + P3*pm3;
 * @param  pline1    原图像P0  P2
 * @param  pline2    原图像P1  P3
 * @param  channels  图像通道数，1=灰阶、3=RGB888彩色
 * @param  u
 * @param  v
 * @return           目标像素值
 * **/
static unsigned char linearInterpolation(unsigned char* pline1, unsigned char* pline2, int channels,int u, int v){
    int pm3 = u * v;
    int pm2 = (u << 8) - pm3;
    int pm1 = (v << 8) - pm3;
    int pm0 = (1 << 16) - pm1 - pm2 - pm3;

    unsigned char result = (unsigned char)(((pm0*pline1[0] + pm2 * pline1[channels] + pm1 * pline2[0] + pm3* pline2[channels]) >> 16)&0xff);

    return result;
}

/**
 * @desc   二次线性插值图像缩放
 * @param  psrc       原图像
 * @param  sw         原图像宽度
 * @param  sh         原图像高度
 * @param  channels   图像通道数，1=灰阶、3=RGB888彩色
 * @param  pdst       缩放后图像
 * @param  dw         缩放后图像宽度
 * @param  dh         缩放后图像高度
 * **/
void zoomImg(unsigned char *psrc, int sw, int sh,int channels, unsigned char *pdst, int dw, int dh)
{
    LOGD("*** zoomImg start *** ");
    if (!psrc || !pdst) return;
    if (sw <= 0 || sh <= 0 || dw <= 0 || dh <= 0) return;
    if (channels != 1 && channels != 3) return;

    unsigned char *p = psrc,*q = pdst,*pt;
    int srcBytesPerLine = sw * channels,dstBytesPerLine = dw * channels;
    int srcy_16 = 0, v_8 = 0, srcx_16 = 0;
    int stepw = (((sw - 1) << 16) / dw +1);
    int steph = ((sh - 1) << 16) / dh + 1;
    int i, j, m;

    srcy_16 = 0;
    for (j = 0; j < dh; j++)
    {
        v_8 = (srcy_16 & 0xFFFF) >> 8;
        p = psrc + srcBytesPerLine * (srcy_16 >> 16);
        srcx_16 = 0;
        for (i = 0; i < dstBytesPerLine; i+=channels)
        {
            pt = &p[(srcx_16>>16)*channels];

            for (m = 0; m < channels; m++) {
                q[i+m] = linearInterpolation(&pt[m], &pt[m] + srcBytesPerLine, channels, (srcx_16 & 0xFFFF) >> 8, v_8);
            }

            srcx_16 += stepw;
        }
        srcy_16 += steph;

        q += dstBytesPerLine;
    }

    LOGD("*** zoomImg complete *** ");
}