#include "StdInc.h"
#include "CResourceLoader.h"
#include "CFileInfo.h"
#include "CLodArchiveLoader.h"
#include "CFilesystemLoader.h"

//For filesystem initialization
#include "../JsonNode.h"
#include "../GameConstants.h"
#include "../VCMIDirs.h"

//experimental support for ERA-style mods. Requires custom config in mod directory
#define ENABLE_ERA_FILESYSTEM

CResourceLoader * CResourceHandler::resourceLoader = nullptr;
CResourceLoader * CResourceHandler::initialLoader = nullptr;

ResourceID::ResourceID()
    :type(EResType::OTHER)
{
}

ResourceID::ResourceID(const std::string & name)
{
	CFileInfo info(name);
	setName(info.getStem());
	setType(info.getType());
}

ResourceID::ResourceID(const std::string & name, EResType::Type type)
{
	setName(name);
	setType(type);
}

ResourceID::ResourceID(const std::string & prefix, const std::string & name, EResType::Type type)
{
	setName(name);
	this->name = prefix + this->name;

	setType(type);
}

std::string ResourceID::getName() const
{
	return name;
}

EResType::Type ResourceID::getType() const
{
	return type;
}

void ResourceID::setName(const std::string & name)
{
	this->name = name;

	size_t dotPos = this->name.find_last_of("/.");

	if(dotPos != std::string::npos && this->name[dotPos] == '.')
		this->name.erase(dotPos);

	// strangely enough but this line takes 40-50% of filesystem loading time
	boost::to_upper(this->name);
}

void ResourceID::setType(EResType::Type type)
{
	this->type = type;
}

CResourceLoader::CResourceLoader()
{
}

CResourceLoader::~CResourceLoader()
{
	// Delete all loader objects
	BOOST_FOREACH ( auto & it, loaders)
	{
		delete it;
	}
}

std::unique_ptr<CInputStream> CResourceLoader::load(const ResourceID & resourceIdent) const
{
	auto resource = resources.find(resourceIdent);

	if(resource == resources.end())
	{
		throw std::runtime_error("Resource with name " + resourceIdent.getName() + " and type "
			+ EResTypeHelper::getEResTypeAsString(resourceIdent.getType()) + " wasn't found.");
	}

	// get the last added resource(most overriden)
	const ResourceLocator & locator = resource->second.back();

	// load the resource and return it
	return locator.getLoader()->load(locator.getResourceName());
}

std::pair<std::unique_ptr<ui8[]>, ui64> CResourceLoader::loadData(const ResourceID & resourceIdent) const
{
	auto stream = load(resourceIdent);
	std::unique_ptr<ui8[]> data(new ui8[stream->getSize()]);
	size_t readSize = stream->read(data.get(), stream->getSize());

	assert(readSize == stream->getSize());
	return std::make_pair(std::move(data), stream->getSize());
}

ResourceLocator CResourceLoader::getResource(const ResourceID & resourceIdent) const
{
	auto resource = resources.find(resourceIdent);

	if (resource == resources.end())
		return ResourceLocator(nullptr, "");
	return resource->second.back();
}

const std::list<ResourceLocator> & CResourceLoader::getResourcesWithName(const ResourceID & resourceIdent) const
{
	static const std::list<ResourceLocator> emptyList;
	auto resource = resources.find(resourceIdent);

	if (resource == resources.end())
		return emptyList;
	return resource->second;
}


std::string CResourceLoader::getResourceName(const ResourceID & resourceIdent) const
{
	auto locator = getResource(resourceIdent);
	if (locator.getLoader())
		return locator.getLoader()->getOrigin() + '/' + locator.getResourceName();
	return "";
}

bool CResourceLoader::existsResource(const ResourceID & resourceIdent) const
{
	// Check if resource is registered
	return resources.find(resourceIdent) != resources.end();
}

void CResourceLoader::addLoader(std::string mountPoint, ISimpleResourceLoader * loader)
{
	loaders.insert(loader);

	// Get entries and add them to the resources list
	const std::list<std::string> & entries = loader->getEntries();

	boost::to_upper(mountPoint);

	BOOST_FOREACH (const std::string & entry, entries)
	{
		CFileInfo file(entry);

		// Create identifier and locator and add them to the resources list
		ResourceID ident(mountPoint, file.getStem(), file.getType());

		//check if entry can be directory. Will only work for filesystem loader but H3 archives don't have dirs anyway.
		if (boost::filesystem::is_directory(loader->getOrigin() + '/' + entry))
			ident.setType(EResType::DIRECTORY);

		ResourceLocator locator(loader, entry);
		resources[ident].push_back(locator);
	}
}

CResourceLoader * CResourceHandler::get()
{
	if(resourceLoader != nullptr)
	{
		return resourceLoader;
	}
	else
	{
		std::stringstream string;
		string << "Error: Resource loader wasn't initialized. "
			   << "Make sure that you set one via CResourceLoaderFactory::setInstance";
		throw std::runtime_error(string.str());
	}
}

//void CResourceLoaderFactory::setInstance(CResourceLoader * resourceLoader)
//{
//	CResourceLoaderFactory::resourceLoader = resourceLoader;
//}

ResourceLocator::ResourceLocator(ISimpleResourceLoader * loader, const std::string & resourceName)
			: loader(loader), resourceName(resourceName)
{

}

ISimpleResourceLoader * ResourceLocator::getLoader() const
{
	return loader;
}

std::string ResourceLocator::getResourceName() const
{
	return resourceName;
}

EResType::Type EResTypeHelper::getTypeFromExtension(std::string extension)
{
	boost::to_upper(extension);

	static const std::map<std::string, EResType::Type> stringToRes =
	        boost::assign::map_list_of
	        (".TXT",   EResType::TEXT)
	        (".JSON",  EResType::TEXT)
	        (".DEF",   EResType::ANIMATION)
	        (".MSK",   EResType::MASK)
	        (".MSG",   EResType::MASK)
	        (".H3C",   EResType::CAMPAIGN)
	        (".H3M",   EResType::MAP)
	        (".FNT",   EResType::FONT)
	        (".BMP",   EResType::IMAGE)
	        (".JPG",   EResType::IMAGE)
	        (".PCX",   EResType::IMAGE)
	        (".PNG",   EResType::IMAGE)
	        (".TGA",   EResType::IMAGE)
	        (".WAV",   EResType::SOUND)
	        (".82M",   EResType::SOUND)
	        (".SMK",   EResType::VIDEO)
	        (".BIK",   EResType::VIDEO)
	        (".MJPG",  EResType::VIDEO)
	        (".MP3",   EResType::MUSIC)
	        (".OGG",   EResType::MUSIC)
	        (".LOD",   EResType::ARCHIVE)
	        (".PAC",   EResType::ARCHIVE)
	        (".VID",   EResType::ARCHIVE)
	        (".SND",   EResType::ARCHIVE)
	        (".VCGM1", EResType::CLIENT_SAVEGAME)
	        (".VLGM1", EResType::LIB_SAVEGAME)
	        (".VSGM1", EResType::SERVER_SAVEGAME);

	auto iter = stringToRes.find(extension);
	if (iter == stringToRes.end())
		return EResType::OTHER;
	return iter->second;
}

std::string EResTypeHelper::getEResTypeAsString(EResType::Type type)
{
#define MAP_ENUM(value) (EResType::value, #value)

	static const std::map<EResType::Type, std::string> stringToRes = boost::assign::map_list_of
		MAP_ENUM(TEXT)
		MAP_ENUM(ANIMATION)
		MAP_ENUM(MASK)
		MAP_ENUM(CAMPAIGN)
		MAP_ENUM(MAP)
		MAP_ENUM(FONT)
		MAP_ENUM(IMAGE)
		MAP_ENUM(VIDEO)
		MAP_ENUM(SOUND)
		MAP_ENUM(MUSIC)
		MAP_ENUM(ARCHIVE)
		MAP_ENUM(CLIENT_SAVEGAME)
		MAP_ENUM(LIB_SAVEGAME)
		MAP_ENUM(SERVER_SAVEGAME)
		MAP_ENUM(DIRECTORY)
		MAP_ENUM(OTHER);

#undef MAP_ENUM

	auto iter = stringToRes.find(type);
	assert(iter != stringToRes.end());

	return iter->second;
}

void CResourceHandler::initialize()
{
	//temporary filesystem that will be used to initialize main one.
	//used to solve several case-sensivity issues like Mp3 vs MP3
	initialLoader = new CResourceLoader;
	resourceLoader = new CResourceLoader;

	auto rootDir = new CFilesystemLoader(GameConstants::DATA_DIR, 3);
	initialLoader->addLoader("GLOBAL/", rootDir);
	initialLoader->addLoader("ALL/", rootDir);

	auto userDir = rootDir;

	//add local directory to "ALL" but only if it differs from root dir (true for linux)
	if (GameConstants::DATA_DIR != GVCMIDirs.UserPath)
	{
		userDir = new CFilesystemLoader(GVCMIDirs.UserPath, 3);
		initialLoader->addLoader("ALL/", userDir);
	}

	//create "LOCAL" dir with current userDir (may be same as rootDir)
	initialLoader->addLoader("LOCAL/", userDir);
}

void CResourceHandler::loadFileSystem(const std::string fsConfigURI)
{
	auto fsConfigData = initialLoader->loadData(ResourceID(fsConfigURI, EResType::TEXT));

	const JsonNode fsConfig((char*)fsConfigData.first.get(), fsConfigData.second);

	BOOST_FOREACH(auto & mountPoint, fsConfig["filesystem"].Struct())
	{
		BOOST_FOREACH(auto & entry, mountPoint.second.Vector())
		{
			tlog5 << "loading resource at " << entry["path"].String() << "\n";

			if (entry["type"].String() == "dir")
			{
				std::string filename = initialLoader->getResourceName(ResourceID(entry["path"].String(), EResType::DIRECTORY));
				if (!filename.empty())
				{
					int depth = 16;
					if (!entry["depth"].isNull())
						depth = entry["depth"].Float();
					resourceLoader->addLoader(mountPoint.first, new CFilesystemLoader(filename, depth));
				}
			}

			if (entry["type"].String() == "file")
			{
				std::string filename = initialLoader->getResourceName(ResourceID(entry["path"].String(), EResType::ARCHIVE));
				if (!filename.empty())
					resourceLoader->addLoader(mountPoint.first, new CLodArchiveLoader(filename));
			}
		}
	}
}

void CResourceHandler::loadModsFilesystems()
{
#ifdef ENABLE_ERA_FILESYSTEM

	auto iterator = initialLoader->getIterator([](const ResourceID & ident) ->  bool
	{
		std::string name = ident.getName();

		return ident.getType() == EResType::TEXT
		    && std::count(name.begin(), name.end(), '/') == 3
		    && boost::algorithm::starts_with(name, "ALL/MODS/")
		    && boost::algorithm::ends_with(name, "FILESYSTEM");
	});

	while (iterator.hasNext())
	{
		tlog1 << "Found mod filesystem: " << iterator->getName() << "\n";
		loadFileSystem(iterator->getName());
		++iterator;
	}
#endif
}
