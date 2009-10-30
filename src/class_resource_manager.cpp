#include <png.h>
#include <allegro.h>
#include "support.h"
#include "class_resource_manager.h"


BITMAP* load_bitmap_from_png(const char* fileName)
{
	FILE* fp = fopen(fileName, "rb");
	if (fp)
	{
		// read header
		unsigned char pngHeader[8];
		fread(&pngHeader, sizeof(unsigned char), 8, fp);
		if (0 != png_sig_cmp(pngHeader, 0, 8))
		{
			fclose(fp);
			//{ char e[256]; sprintf(e, "%s is not a valid PNG file!", fileName); LogFatal(e); }
		}

		// create the png structures we need for reading
		// it is not apparent, but these are all pointers
		// can thank the idiots that made libpng for the shit typedef names
		png_structp pngReadStruct;
		png_infop 	pngInfoStruct;
		png_infop 	pngEndStruct;

		pngReadStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
		if (!pngReadStruct)
		{
			fclose(fp);
			//LogFatal("Cannot create the PNG Read Structure!");
		}

		pngInfoStruct = png_create_info_struct(pngReadStruct);
		if (!pngInfoStruct)
		{
			fclose(fp);
			png_destroy_read_struct(&pngReadStruct, 0, 0);
			//LogFatal("Cannot create the PNG Info Structure!");
		}

		pngEndStruct = png_create_info_struct(pngReadStruct);
		if (!pngEndStruct)
		{
			fclose(fp);
			png_destroy_read_struct(&pngReadStruct, &pngInfoStruct, 0);
			//LogFatal("Cannot create the PNG Info Structure!");
		}

		png_init_io(pngReadStruct, fp);
		png_set_sig_bytes(pngReadStruct, 8);

		// read dimensions
		unsigned long imageWidth = 0;
		unsigned long imageHeight = 0;
		int imageBitsPerPixel = 0;
		int imageColorType = 0;

		png_read_info(pngReadStruct, pngInfoStruct);
		png_get_IHDR(pngReadStruct,
			pngInfoStruct,
			&imageWidth,
			&imageHeight,
			&imageBitsPerPixel,
			&imageColorType,
			0,
			0,
			0);

		png_set_packing(pngReadStruct);

		// expand low-bpp gray-scale to 8 bit
		if ((imageColorType == PNG_COLOR_TYPE_GRAY) && (imageBitsPerPixel < 8))
		{
			png_set_expand(pngReadStruct);
		}

		// convert gray-scale to RGB
		if ((imageColorType == PNG_COLOR_TYPE_GRAY) || (imageColorType == PNG_COLOR_TYPE_GRAY_ALPHA))
		{
			png_set_gray_to_rgb(pngReadStruct);
		}

		// if there is transparency info, then create an alpha channel
		bool transparencyToAlpha = false;
		if (png_get_valid(pngReadStruct, pngInfoStruct, PNG_INFO_tRNS))
		{
			png_set_tRNS_to_alpha(pngReadStruct);
			transparencyToAlpha = true;
		}

		// convert 16 bit to 8 bit
		if (16 == imageBitsPerPixel)
		{
			png_set_strip_16(pngReadStruct);
		}

		// read pixels
		unsigned long imageBytesPerRow = png_get_rowbytes(pngReadStruct, pngInfoStruct);

		imageBitsPerPixel = imageBytesPerRow * 8 / imageWidth;

		imageBitsPerPixel = (imageBitsPerPixel < 8) ? 8 : imageBitsPerPixel;

		BITMAP* resource = 0;
		resource = create_bitmap_ex(imageBitsPerPixel, imageWidth, imageHeight);
		if (0 == resource)
		{
			png_read_end(pngReadStruct, pngEndStruct);
			png_destroy_read_struct(&pngReadStruct, &pngInfoStruct, &pngEndStruct);
			fclose(fp);
			//LogFatal("Could not create the Allegro bitmap surface!");
		}

		/*int scanPasses = */png_set_interlace_handling(pngReadStruct);

		if ((24 == imageBitsPerPixel) || (32 == imageBitsPerPixel))
		{
			int c = makecol_depth(imageBitsPerPixel, 0, 0, 255);
			unsigned char *pc = (unsigned char *)&c;
			if (pc[0] == 255)
			{
				png_set_bgr(pngReadStruct);
			}
#if defined(ALLEGRO_BIG_ENDIAN)
			png_set_swap_alpha(pngReadStruct);
#endif
		}

		for (unsigned long y = 0; y < imageHeight; y++)
		{
			png_read_row(pngReadStruct, resource->line[y], 0);
		}

		png_read_end(pngReadStruct, pngEndStruct);
		png_destroy_read_struct(&pngReadStruct, &pngInfoStruct, &pngEndStruct);
		fclose(fp);
		return resource;
	}
	else
	{
		//{ char e[256]; sprintf(e, "Could not open %s!", fileName); LogFatal(e); }
	}
	return 0;
}


////////////////////////////////////////////////////////////////////////////////
bool is_image(const char* name)
{
	std::string n = name;
	n = n.substr(n.size() - 3);
	std::transform(n.begin(), n.end(), n.begin(), tolower);
	return "png" == n;
}
////////////////////////////////////////////////////////////////////////////////

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	std::vector <Resource*>::iterator iter = resources_.begin();
	while(iter != resources_.end())
	{
		if (*iter)
		{
			fprintf(stderr,"releasing resource %s\n", (*iter)->name_.c_str());
			delete *iter;
			resources_.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	table_.clear();
}

void ResourceManager::load(const char* resname)
{
	unsigned int id = resources_.size();

	if (is_image(resname))
	{
		ImageResource* resource = new ImageResource;
		resource->image_ = load_bitmap_from_png(resname); //load_bitmap(resname, 0);
		if (resource)
		{
			resource->name_ = resname;
			resources_.push_back(resource);
			fprintf(stderr, "loaded %s\n", resname);
		}
		else
		{
			fprintf(stderr, "error cannot load %s\n", resname);
			return;
		}
	}
	else
	{
		fprintf(stderr, "error loading %s - not an image\n", resname);
		return;
	}

	table_[resname] = id;
}

Resource* ResourceManager::get(const char* resname)
{
	// very dangerous if the resource name is not right
	return resources_[table_[resname]];
}
