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


#define TAG "native" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型

JNIEXPORT jstring JNICALL
Java_com_github_fwh007_ndktest_MainActivity_getMacAddress2
        (JNIEnv *env, jobject obj) {
    struct ifreq s;
    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

    strcpy(s.ifr_name, "eth0");
    char result[64] = {};
    int io = ioctl(fd, SIOCGIFFLAGS, &s);
    __android_log_print(ANDROID_LOG_DEBUG, "native", "io -> \t\t%d\n", io);
    if (io == 0) {
        int i;
        for (i = 0; i < 6; ++i) {
            char temp[3];
            sprintf(temp, "%02x:", s.ifr_addr.sa_data[i]);
//            __android_log_print(ANDROID_LOG_DEBUG, temp, "aaa\n");
            __android_log_print(ANDROID_LOG_DEBUG, "native", "temp -> \t\t%s", temp);
            __android_log_print(ANDROID_LOG_DEBUG, "native", "temp -> \t\t%s", result);
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
    __android_log_print(ANDROID_LOG_DEBUG, "native", "result -> \t%s", result);

    return (*env)->NewStringUTF(env, result);
}

JNIEXPORT jstring JNICALL
Java_com_github_fwh007_ndktest_MainActivity_getMacAddress
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
        __android_log_print(ANDROID_LOG_DEBUG, "native", "Something wrong.");
    }

    struct ifreq *it = ifc.ifc_req;
    const struct ifreq *const end = it + (ifc.ifc_len / sizeof(struct ifreq));

    for (; it != end; ++it) {
        strcpy(ifr.ifr_name, it->ifr_name);
        int io = ioctl(sock, SIOCGIFFLAGS, &ifr);
        __android_log_print(ANDROID_LOG_DEBUG, "native", "io -> \t\t%d\n", io);
        if (io == 0) {
            if (!(ifr.ifr_flags /*& IFF_LOOPBACK*/)) { // don't count loopback
                __android_log_print(ANDROID_LOG_DEBUG, "native", "io -> \t\t%d\n", io);
                if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
                    __android_log_print(ANDROID_LOG_DEBUG, "native", "io -> \t\t%d\n", io);
                    success = 1;
                    break;
                }
            }
        } else { /* handle error */ }
    }

    unsigned char mac_address[6] = {};

    __android_log_print(ANDROID_LOG_DEBUG, "native", "su -> \t\t%s\n", success);
    __android_log_print(ANDROID_LOG_DEBUG, "native", "su -> \t\t%s\n", ifr.ifr_hwaddr.sa_data);
    if (success) memcpy(mac_address, ifr.ifr_hwaddr.sa_data, 6);

    __android_log_print(ANDROID_LOG_DEBUG, "native", "add -> \t\t%s\n", mac_address);
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
        struct sockaddr_in *sin = (struct sockaddr_in *)&r->ifr_addr;
        if (!strcmp(r->ifr_name, "lo"))
            continue; // skip loopback interface

        // get MAC address
        if(ioctl(s, SIOCGIFHWADDR, r) < 0) {
            __android_log_print(ANDROID_LOG_DEBUG, "native", "5 wrong.");
        }

        char macaddr[18];
        sprintf(macaddr, " %02X:%02X:%02X:%02X:%02X:%02X",
                (unsigned char)r->ifr_hwaddr.sa_data[1],
                (unsigned char)r->ifr_hwaddr.sa_data[0],
                (unsigned char)r->ifr_hwaddr.sa_data[2],
                (unsigned char)r->ifr_hwaddr.sa_data[3],
                (unsigned char)r->ifr_hwaddr.sa_data[4],
                (unsigned char)r->ifr_hwaddr.sa_data[5]);
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
            case AF_INET:
            {
                printf("%s  address family: %d%s\n", ifa->ifa_name, family, " AF_INET");
                struct sockaddr_in *s = (struct sockaddr_in *) ifa->ifa_addr;
                //char * aip = inet_ntoa(s->sin_addr);
                char aip[16];
                inet_ntop(AF_INET,&s->sin_addr,aip,sizeof(aip));
                LOGD("ipv4 -> \t\t%s\n", aip);
//                printf("\tIPv4 Address:<%s>\n", aip);
                break;
            }
            case AF_INET6:
            {
                printf("%s  address family: %d%s\n", ifa->ifa_name, family, " AF_INET6");
                struct sockaddr_in6 *s = (struct sockaddr_in6 *) ifa->ifa_addr;
                //char * aip = inet_ntoa(s->sin_addr);
                char aip[40];
                inet_ntop(AF_INET6,&s->sin6_addr,aip,sizeof(aip));
                LOGD("ipv6 -> \t\t%s\n", aip);
//                printf("\tIPv4 Address:<%s>\n", aip);
                break;
            }
            default:
            {
                LOGD("Unknown address family");
//                printf("Unknown address family\n");
                break;
            }
        }
    }

    return (*env)->NewStringUTF(env, "end");
}