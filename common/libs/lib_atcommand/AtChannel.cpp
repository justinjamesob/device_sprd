#define LOG_TAG "IAtChannel"

#include <android/log.h>
#include <binder/IServiceManager.h>
#include <utils/String8.h>
#include <utils/String16.h>
#include "IAtChannel.h"
#include "AtChannel.h"

using namespace android;

const int MAX_SERVICE_NAME = 100;

static int getPhoneId(int modemId, int simId)
{
    return 0;
}

static String16 getServiceName(int modemId, int simId)
{
    char serviceName[MAX_SERVICE_NAME];
    memset(serviceName, 0, sizeof(serviceName));
    snprintf(serviceName, MAX_SERVICE_NAME - 1, "atchannel%d", getPhoneId(modemId, simId));
    return String16(serviceName);
}

const char* sendAt(int modemId, int simId, const char* atCmd)
{
    sp<IServiceManager> sm = defaultServiceManager();
    if (sm == NULL) {
        ALOGE("Couldn't get default ServiceManager\n");
        return "ERROR";
    }

    sp<IAtChannel> atChannel;
    String16 serviceName = getServiceName(modemId, simId);
    atChannel = interface_cast<IAtChannel>(sm->getService(serviceName));
    if (atChannel == NULL) {
        ALOGE("Couldn't get connection to %s\n", String8(serviceName).string());
        return "ERROR";
    }

    return atChannel->sendAt(atCmd);
}
