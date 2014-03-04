#include "StdInc.h"
#include "launcherdirs.h"

#include "../lib/VCMIDirs.h"

static CLauncherDirs launcherDirsGlobal;

CLauncherDirs::CLauncherDirs()
{
	QDir().mkdir(downloadsPath());
	QDir().mkdir(modsPath());
}

CLauncherDirs & CLauncherDirs::get()
{
	return launcherDirsGlobal;
}

QString CLauncherDirs::downloadsPath()
{
	return QString::fromUtf8(VCMIDirs::get().userCachePath().c_str()) + "/downloads";
}

QString CLauncherDirs::modsPath()
{
	return QString::fromUtf8(VCMIDirs::get().userDataPath().c_str()) + "/Mods";
}
