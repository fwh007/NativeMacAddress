//
// Created by gzfengwenhao on 2017/7/13 0013.
//

#include "native-lib.h"
#include <net/if.h>
#include <jni.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <Android/log.h>
#include <stdio.h>
#include <stdio.h>
#include <sys/socket.h>
#include <my-ifaddrs.h>
#include <netpacket/packet.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if_arp.h>
#include <sys/system_properties.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>


#define TAG "native" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型

JNIEXPORT jstring JNICALL
Java_com_github_fwh007_ndktest_MainActivity_getMacAddress1
        (JNIEnv *env, jobject obj) {
    struct ifreq s;
    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

    strcpy(s.ifr_name, "eth0");
    char result[64] = {};
    int io = ioctl(fd, SIOCGIFFLAGS, &s);
    LOGD("io -> \t\t%d\n", io);
    if (io == 0) {
        int i;
        for (i = 0; i < 6; ++i) {
            char temp[3];
            sprintf(temp, "%02x:", s.ifr_addr.sa_data[i]);
//            __android_log_print(ANDROID_LOG_DEBUG, temp, "aaa\n");
            LOGD("temp -> \t\t%s", temp);
            LOGD("temp -> \t\t%s", result);
            strcat(result, temp);
//            const char c = s.ifr_addr.sa_data[i];
//            printf(" %02x", (unsigned char) s.ifr_addr.sa_data[i]);
//            strcpy(result, c);
//            __android_log_print(ANDROID_LOG_DEBUG, "MMM", "test");
//            LOGD("Call String Log!\n");
//            strcpy(result, s.ifr_addr.sa_data[i]);
        }
        puts("\n");
    }
//    LOGD("Call String Log!\n");
//    LOGD(result);
    LOGD("result -> \t%s", result);

    return (*env)->NewStringUTF(env, result);
}

JNIEXPORT jstring JNICALL
Java_com_github_fwh007_ndktest_MainActivity_getMacAddress2
        (JNIEnv *env, jobject obj) {
    struct ifreq ifr;
    struct ifconf ifc;
    char buf[1024];
    int success = 0;

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock == -1) { /* handle error*/ };

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) {
        LOGD("Something wrong.");
    }

    struct ifreq *it = ifc.ifc_req;
    const struct ifreq *const end = it + (ifc.ifc_len / sizeof(struct ifreq));

    for (; it != end; ++it) {
        strcpy(ifr.ifr_name, it->ifr_name);
        LOGD("ifr_name -> \t%s\n", ifr.ifr_name);
//        if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
        if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
//                if (!(ifr.ifr_flags & IFF_LOOPBACK)) { // don't count loopback
            if (ifr.ifr_addr.sa_family != ARPHRD_LOOPBACK) { // don't count loopback
                LOGD("io3 -> \t\t%s\n", ifr.ifr_hwaddr.sa_data);
                success = 1;
                break;
            }
        }
//        } else { /* handle error */ }
    }

    char mac_address[18] = {};

    LOGD("su -> \t\t%d\n", success);
    LOGD("su -> \t\t%s\n", ifr.ifr_hwaddr.sa_data);
    if (success) {
        sprintf(mac_address, "%X:%X:%X:%X:%X:%X",
                (unsigned char) ifr.ifr_hwaddr.sa_data[0],
                (unsigned char) ifr.ifr_hwaddr.sa_data[1],
                (unsigned char) ifr.ifr_hwaddr.sa_data[2],
                (unsigned char) ifr.ifr_hwaddr.sa_data[3],
                (unsigned char) ifr.ifr_hwaddr.sa_data[4],
                (unsigned char) ifr.ifr_hwaddr.sa_data[5]);
    }
    close(sock);

    LOGD("result -> \t%s\n", mac_address);
    return (*env)->NewStringUTF(env, mac_address);
}


JNIEXPORT jstring JNICALL
Java_com_github_fwh007_ndktest_MainActivity_getMacAddress3
        (JNIEnv *env, jobject obj) {
    struct ifreq *ifr;
    struct ifconf ifc;
    int s, i;
    int numif;

    // find number of interfaces.
    memset(&ifc, 0, sizeof(ifc));
    ifc.ifc_ifcu.ifcu_req = NULL;
    ifc.ifc_len = 0;

//    if ((s = ::socket(PF_INET, SOCK_STREAM, 0)) < 0) {
//        __android_log_print(ANDROID_LOG_DEBUG, "native", "1 wrong.");
//
//    }

    if (ioctl(s, SIOCGIFCONF, &ifc) < 0) {
        __android_log_print(ANDROID_LOG_DEBUG, "native", "2 wrong.");
    }

//    if ((ifr = (ifreq*)malloc(ifc.ifc_len)) == NULL) {
//        __android_log_print(ANDROID_LOG_DEBUG, "native", "3 wrong.");
//    }

    ifc.ifc_ifcu.ifcu_req = ifr;

    if (ioctl(s, SIOCGIFCONF, &ifc) < 0) {
        __android_log_print(ANDROID_LOG_DEBUG, "native", "4 wrong.");
    }

    numif = ifc.ifc_len / sizeof(struct ifreq);

    for (i = 0; i < numif; i++) {
        struct ifreq *r = &ifr[i];
        struct sockaddr_in *sin = (struct sockaddr_in *) &r->ifr_addr;
        if (!strcmp(r->ifr_name, "lo"))
            continue; // skip loopback interface

        // get MAC address
        if (ioctl(s, SIOCGIFHWADDR, r) < 0) {
            __android_log_print(ANDROID_LOG_DEBUG, "native", "5 wrong.");
        }

        char macaddr[18];
        sprintf(macaddr, " %02X:%02X:%02X:%02X:%02X:%02X",
                (unsigned char) r->ifr_hwaddr.sa_data[1],
                (unsigned char) r->ifr_hwaddr.sa_data[0],
                (unsigned char) r->ifr_hwaddr.sa_data[2],
                (unsigned char) r->ifr_hwaddr.sa_data[3],
                (unsigned char) r->ifr_hwaddr.sa_data[4],
                (unsigned char) r->ifr_hwaddr.sa_data[5]);
        __android_log_print(ANDROID_LOG_DEBUG, "native", "result -> \t\t%s\n", macaddr);
    }
    close(s);
    return (*env)->NewStringUTF(env, "nli");
}

JNIEXPORT jstring JNICALL
Java_com_github_fwh007_ndktest_MainActivity_getMacAddress4
        (JNIEnv *env, jobject obj) {
    struct ifaddrs *ifaddr, *ifa;
    int family;

    LOGD("111");
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");

        return (*env)->NewStringUTF(env, "nli");
    }

    LOGD("222");
    /* Walk through linked list, maintaining head pointer so we
       can free list later */
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        LOGD("333");
        family = ifa->ifa_addr->sa_family;

        switch (family) {
            case AF_PACKET: {
                printf("%s  address family: %d%s\n", ifa->ifa_name, family, " AF_PACKET");
                struct sockaddr_ll *s = (struct sockaddr_ll *) ifa->ifa_addr;
                for (int i = 0; i < 6; i++) {
                    LOGD("mac -> \t\t%s\n", s->sll_addr[i]);
//                    printf("%s%02X%s", i == 0 ? "\tMAC Address:<" : "", s->sll_addr[i],
//                           i < 5 ? ":" : ">\n");
                }
                break;
            }
            case AF_INET: {
                printf("%s  address family: %d%s\n", ifa->ifa_name, family, " AF_INET");
                struct sockaddr_in *s = (struct sockaddr_in *) ifa->ifa_addr;
                //char * aip = inet_ntoa(s->sin_addr);
                char aip[16];
                inet_ntop(AF_INET, &s->sin_addr, aip, sizeof(aip));
                LOGD("ipv4 -> \t\t%s\n", aip);
//                printf("\tIPv4 Address:<%s>\n", aip);
                break;
            }
            case AF_INET6: {
                printf("%s  address family: %d%s\n", ifa->ifa_name, family, " AF_INET6");
                struct sockaddr_in6 *s = (struct sockaddr_in6 *) ifa->ifa_addr;
                //char * aip = inet_ntoa(s->sin_addr);
                char aip[40];
                inet_ntop(AF_INET6, &s->sin6_addr, aip, sizeof(aip));
                LOGD("ipv6 -> \t\t%s\n", aip);
//                printf("\tIPv4 Address:<%s>\n", aip);
                break;
            }
            default: {
                LOGD("Unknown address family");
//                printf("Unknown address family\n");
                break;
            }
        }
    }

    return (*env)->NewStringUTF(env, "end");
}

JNIEXPORT jstring JNICALL
Java_com_github_fwh007_ndktest_MainActivity_getIMEI
        (JNIEnv *env, jobject obj) {
    //returns the string length of the value.
    char imei_start[16];
    char g_imei[1024];
    int ir = __system_property_get("ro.gsm.imei", imei_start);

    if (ir > 0) {
        imei_start[15] = 0;//strz end
        printf("method1 got imei %s len %d\r\n", imei_start, strlen(imei_start));
        strcpy(g_imei, imei_start);
    } else {
        printf("method1 imei failed - trying method2\r\n");
        //old dumpsys imei getter
        char *res = exec_get_out("dumpsys iphonesubinfo");
        const char *imei_start_match = "ID = ";
        int imei_start_match_len = strlen(imei_start_match);
        char *imei_start = strstr(res, imei_start_match);
        if (imei_start && strlen(imei_start) >= 15 + imei_start_match_len) {
            imei_start += imei_start_match_len;
            imei_start[15] = 0;
            printf("method2 IMEI [%s] len %d\r\n", imei_start, strlen(imei_start));
            strcpy(g_imei, imei_start);
        }
    }
    return (*env)->NewStringUTF(env, "end");
}

JNIEXPORT jstring JNICALL
Java_com_github_fwh007_ndktest_MainActivity_getDeviceId
        (JNIEnv *env, jobject obj) {
    char a[18] = {};
    int b = 100;
    LOGD("aaa -> %d", b);
    b = execl("/system/bin/settings", "settings", " get secure android_id", (char *) NULL);
    LOGD("aaa -> %d", b);
    b = system("settings get secure android_id");
    LOGD("aaa -> %d", b);
    LOGD("aaa -> %d", WEXITSTATUS(b));

    char *c = "";
    FILE *fpRead;
    fpRead = popen("ls ./ | wc -l", "r");
    if (fpRead == NULL) {
        LOGD("ccc -> %s", "ERROR");
    }
    char buf[1024];
    memset(buf, '\0', sizeof(buf));
    while (fgets(buf, 1024 - 1, fpRead) != NULL) {
        LOGD("ccc1 -> %s", buf);
        c = buf;
    }
    if (fpRead != NULL)
        LOGD("ccc2 -> %d", pclose(fpRead));
    LOGD("ccc3 -> %s", c);
    return (*env)->NewStringUTF(env, a);
}

JNIEXPORT jstring JNICALL
Java_com_github_fwh007_ndktest_MainActivity_getMacAddress5
        (JNIEnv *env, jobject obj) {
    char result_buf[1024] = {};
    char command[1024];
    int rc = 0; // 用于接收命令返回值
    FILE *fp;

    /*将要执行的命令写入buf*/
    snprintf(command, sizeof(command), "cat /sys/class/net/wlan0/address");

    /*执行预先设定的命令，并读出该命令的标准输出*/
    fp = popen(command, "r");
    if (NULL == fp) {
        LOGD("popen执行失败！", "");
        exit(1);
    }
    while (fgets(result_buf, sizeof(result_buf), fp) != NULL) {
        /*为了下面输出好看些，把命令返回的换行符去掉*/
        if ('\n' == result_buf[strlen(result_buf) - 1]) {
            result_buf[strlen(result_buf) - 1] = '\0';
        }
        LOGD("命令【%s】 输出【%s】\r\n", command, result_buf);
    }

    /*等待命令执行完毕并关闭管道及文件指针*/
    rc = pclose(fp);
    if (-1 == rc) {
        LOGD("关闭文件指针失败", "");
        exit(1);
    } else {
        LOGD("命令【%s】子进程结束状态【%d】命令返回值【%d】\r\n", command, rc, WEXITSTATUS(rc));
    }
    return (*env)->NewStringUTF(env, result_buf);
}

JNIEXPORT jstring JNICALL
Java_com_github_fwh007_ndktest_MainActivity_getAndroidId
        (JNIEnv *env, jobject obj, jobject ctx) {
    jclass contextClass = (*env)->GetObjectClass(env, ctx);

    jmethodID getContentResolverMID = (*env)->GetMethodID(env, contextClass, "getContentResolver",
                                                          "()Landroid/content/ContentResolver;");

    jobject contentResolverObj = (*env)->CallObjectMethod(env, ctx, getContentResolverMID);

    jclass settingsSecureClass = (*env)->FindClass(env, "android/provider/Settings$Secure");

    jmethodID getStringMID = (*env)->GetStaticMethodID(env, settingsSecureClass, "getString",
                                                       "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;");

    jstring idStr = (*env)->NewStringUTF(env, "android_id");
    jstring androidId = (jstring) (*env)->CallStaticObjectMethod(env, settingsSecureClass,
                                                                 getStringMID, contentResolverObj,
                                                                 idStr);
    return androidId;
}

JNIEXPORT jstring JNICALL
Java_com_github_fwh007_ndktest_MainActivity_getMacAddress6
        (JNIEnv *env, jobject obj, jobject ctx) {

    jclass idUtilClass = (*env)->FindClass(env, "com/github/fwh007/ndktest/IDUtil");

    jmethodID getStringMID = (*env)->GetStaticMethodID(env, idUtilClass, "getWifiMacAddress",
                                                       "()Ljava/lang/String;");

    jstring macAddress = (jstring) (*env)->CallStaticObjectMethod(env, idUtilClass, getStringMID);

    return macAddress;
}