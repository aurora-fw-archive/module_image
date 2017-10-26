/****************************************************************************
** ┌─┐┬ ┬┬─┐┌─┐┬─┐┌─┐  ┌─┐┬─┐┌─┐┌┬┐┌─┐┬ ┬┌─┐┬─┐┬┌─
** ├─┤│ │├┬┘│ │├┬┘├─┤  ├┤ ├┬┘├─┤│││├┤ ││││ │├┬┘├┴┐
** ┴ ┴└─┘┴└─└─┘┴└─┴ ┴  └  ┴└─┴ ┴┴ ┴└─┘└┴┘└─┘┴└─┴ ┴
** A Powerful General Purpose Framework
** More information in: https://aurora-fw.github.io/
**
** Copyright (C) 2017 Aurora Framework, All rights reserved.
**
** This file is part of the Aurora Framework. This framework is free
** software; you can redistribute it and/or modify it under the terms of
** the GNU Lesser General Public License version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE included in
** the packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
****************************************************************************/

#ifndef AURORAFW_IMAGE_IMAGE_H
#define AURORAFW_IMAGE_IMAGE_H

#include <AuroraFW/Global.h>
#include <AuroraFW/CLI/Log.h>
#include <AuroraFW/STDL/STL/IOStream.h>
#include <AuroraFW/GEngine/Color.h>

#include <exception>
#include <FreeImage.h>

namespace AuroraFW {
	namespace ImageManager {
		enum ImageFlags {
			Read = 		(1<<0),
			Write =		(1<<1)
		};

		inline ImageFlags operator|(ImageFlags a, ImageFlags b)
		{
			return static_cast<ImageFlags>(static_cast<int>(a) | static_cast<int>(b));
		}

		inline ImageFlags operator&(ImageFlags a, ImageFlags b)
		{
			return static_cast<ImageFlags>(static_cast<int>(a) & static_cast<int>(b));
		}

		inline ImageFlags operator~(ImageFlags a)
		{
			return static_cast<ImageFlags>(~static_cast<int>(a));
		}

		class ImageNotFoundException: public std::exception
		{
		private:
			const std::string _path;
		public:
			ImageNotFoundException(const char *);
			virtual const char* what() const throw();
		};

		class ImageAllocationFailedException: public std::exception
		{
		private:
			const std::string _path;
		public:
			ImageAllocationFailedException(const char *);
			virtual const char* what() const throw();
		};

		class ImageIsReadOnlyException: public std::exception
		{
		public:
			ImageIsReadOnlyException();
			virtual const char* what() const throw();
		};

		class ImageIsWriteOnlyException: public std::exception
		{
		public:
			ImageIsWriteOnlyException();
			virtual const char* what() const throw();
		};

		class AFW_EXPORT Image
		{
		public:
			Image(FREE_IMAGE_FORMAT , const char* , ImageFlags = (ImageFlags::Read | ImageFlags::Write),
			int = AFW_DONTCARE , int = AFW_DONTCARE , int = 32);
			~Image();
		
			bool isReadOnly() const;
			bool isWriteOnly() const;
			bool isReadAndWrite() const;

			bool is32Bit() const;
			void convertTo32Bits();

			void setFlags(const ImageFlags );
			void setReadOnly();
			void setWriteOnly();
			void setReadAndWrite();

			void setClearPixelColor(const GEngine::Color& );
			void clearPixel(int , int );
			void clearImage();

			bool drawPixel(int , int , const GEngine::Color& );

			bool saveImage();
			
		private:
			FREE_IMAGE_FORMAT _fif;
			const char *_path;
			ImageFlags _flags;
			int _width, _height, _bpp;

			static bool _freeImageInitialised;
			FIBITMAP *_image;
			GEngine::Color _clearColor = GEngine::Color(0xFFFFFF);
			RGBQUAD _color;
		};

		// Inline definitions
		inline bool Image::isReadOnly() const
		{
			return (_flags & ImageFlags::Read) == ImageFlags::Read;
		}

		inline bool Image::isWriteOnly() const
		{
			return (_flags & ImageFlags::Write) == ImageFlags::Write;
		}

		inline bool Image::isReadAndWrite() const
		{
			return (_flags & (ImageFlags::Read | ImageFlags::Write)) == (ImageFlags::Read | ImageFlags::Write);
		}

		inline bool Image::is32Bit() const
		{
			return _bpp == 32;
		}

		inline void Image::setFlags(const ImageFlags flags)
		{
			_flags = flags;
		}
	}
}

#endif	// AURORAFW_IMAGE_IMAGE_H