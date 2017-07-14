#include <net/if.h>
#include <jni.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <Android/log.h>

#define TAG "myDemo-jni" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型

extern "C" {
JNIEXPORT jstring JNICALL
Java_com_github_fwh007_ndktest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

JNIEXPORT jstring JNICALL
Java_com_github_fwh007_ndktest_MainActivity_getMacAddress(
        JNIEnv *env, jobject
) {
    struct ifreq *ifr;
    struct ifconf *ifc;
    char buf[1024];
    int success = 0;

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock == -1) { /* handle error*/ };

    ifc->ifc_len = sizeof(buf);
    ifc->ifc_buf = buf;
    if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) { /* handle error */ }

    struct ifreq *it = ifc->ifc_req;
    const struct ifreq *const end = it + (ifc->ifc_len / sizeof(struct ifreq));

    for (; it != end; ++it) {
        strcpy(ifr->ifr_name, it->ifr_name);
        if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
            if (!(ifr->ifr_flags /*& IFF_LOOPBACK*/)) { // don't count loopback
                if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
                    success = 1;
                    break;
                }
            }
        } else { /* handle error */ }
    }

    if (success) {

        char mac_address[6];
        memcpy(mac_address, ifr->ifr_hwaddr.sa_data, 6);

        return env->NewStringUTF(mac_address);
    } else {
        return env->NewStringUTF("false");
    }
}


JNIEXPORT jstring JNICALL
Java_com_github_fwh007_ndktest_MainActivity_getMacAddress2(
        JNIEnv *env, jobject
) {
    struct ifreq s;
    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

    strcpy(s.ifr_name, "eth0");
    char result[64] = "aaaaaaaaaaaaaaaaaaaaaa";
    if (0 == ioctl(fd, SIOCGIFHWADDR, &s)) {
        int i;
        for (i = 0; i < 6; ++i) {
            unsigned char c = s.ifr_addr.sa_data[i];
            printf(" %02x", (unsigned char) s.ifr_addr.sa_data[i]);
            LOGD("MMM  %s", "test");
//            strcpy(result, s.ifr_addr.sa_data[i]);
        }
        puts("\n");
    }
    return env->NewStringUTF(result);;
}
}
