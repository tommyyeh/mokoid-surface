#define LOG_TAG "Demo"

#include <utils/Log.h>
#include <utils/misc.h>

#include <ui/SurfaceComposerClient.h>
#include <ui/Region.h>
#include <ui/Rect.h>

#include "surfaceflinger.h"

using namespace android;

static sp<SurfaceComposerClient> android_client;
static sp<Surface> android_surface;

char *surfaceflinger_lock(void)
{
	Surface::SurfaceInfo info;

	android_surface->lock(&info);

	return (char *) info.bits;
}

void surfaceflinger_unlock(void)
{
	android_surface->unlockAndPost();
}

void surfaceflinger_fini(void)
{
	android_surface = NULL;
	android_client = NULL;
}

int surfaceflinger_init(int x, int y, int w, int h, int *stride)
{
	int depth = 32;
	int fmt;

    if (depth == 32)
        fmt = PIXEL_FORMAT_RGBA_8888;
    else
        fmt = PIXEL_FORMAT_RGB_565;

	android_client = new SurfaceComposerClient;
	if (android_client == NULL) {
		LOGE("failed to create client\n");
		return 0;
	}

	android_surface = android_client->createSurface(getpid(), 0, w, h, fmt, 0);
	if (android_surface == NULL) {
		LOGE("failed to create surface\n");
		return 0;
	}

	android_client->openTransaction();
	android_surface->setPosition(x, y);
	android_surface->setLayer(INT_MAX);  // z-index
	android_client->closeTransaction();

	return 1;
}
