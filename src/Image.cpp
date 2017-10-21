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

#include <AuroraFW/Image/Image.h>

namespace AuroraFW {
	namespace ImageManager {

		bool Image::_freeImageInitialised = false;

		ImageNotFoundException::ImageNotFoundException(const char *path)
			: _path(std::string("The file " + std::string(path) + " couldn't be found/read!")) {}

		const char* ImageNotFoundException::what() const throw()
		{
			return _path.c_str();
		}

		ImageAllocationFailedException::ImageAllocationFailedException(const char *path)
			: _path(std::string("The allocation for the image " + std::string(path) + " failed.")) {}

		const char* ImageAllocationFailedException::what() const throw()
		{
			return _path.c_str();
		}

		ImageIsReadOnlyException::ImageIsReadOnlyException() {}

		const char* ImageIsReadOnlyException::what() const throw()
		{
			return "There was an attempt to edit a read-only image!";
		}

		ImageIsWriteOnlyException::ImageIsWriteOnlyException() {}
		
		const char* ImageIsWriteOnlyException::what() const throw()
		{
			return "There was an attempt to read a write-only image!";
		}

		Image::Image(FREE_IMAGE_FORMAT fif, const char* path , ImageFlags flags , int width , int height, int bpp)
			: _fif(fif), _path(path), _flags(flags), _width(width), _height(height), _bpp(bpp)
		{
			// Initializes FreeImage
			if(!_freeImageInitialised) {
				FreeImage_Initialise();
				CLI::Log(CLI::Information, "FreeImage was initialized.");
				_freeImageInitialised = true;
			}

			if(_flags & ImageFlags::Read) {
				// The user wants to read the file.
				AuroraFW::Debug::Log("Read flag.");
				_image = FreeImage_Load(fif, path, 0);
				if(!_image) {
					throw ImageNotFoundException(path);
				}
			}

			if(_flags & ImageFlags::Write) {
				if(_image == nullptr) {
					_image = FreeImage_Allocate(_width, _height, _bpp);
					if(_image == nullptr) {
						throw ImageAllocationFailedException(path);
					}
					AuroraFW::Debug::Log("Write flag: image didn't exist, space was allocated.");
				}
			}
		}

		Image::~Image()
		{
			delete _image;
			AuroraFW::Debug::Log("The image was deleted from memory.");
		}

		void Image::setFlags(ImageFlags flags)
		{
			_flags = flags;
		}

		void Image::setReadOnly()
		{
			_flags = _flags & ~ImageFlags::Write;
			_flags = _flags | ImageFlags::Read;
		}

		void Image::setWriteOnly()
		{
			_flags = _flags & ~ImageFlags::Read;
			_flags = _flags | ImageFlags::Write;
		}

		void Image::setReadAndWrite()
		{
			_flags = _flags | (ImageFlags::Read | ImageFlags::Write);
		}

		void Image::setClearPixelColor(GEngine::Color color)
		{
			// TODO: Implement
		}

		void Image::clearPixel(int x, int y)
		{
			// TODO: Implement
		}

		void Image::clearImage()
		{
			// TODO: Implement
		}

		bool Image::drawPixel(int x, int y, GEngine::ColorF& color)
		{
			if(isReadOnly())
				throw ImageIsReadOnlyException();
			
			_color.rgbRed = color.redF();
			_color.rgbGreen = color.greenF();
			_color.rgbBlue = color.blueF();
			return FreeImage_SetPixelColor(_image, x, y, &_color);
		}

		bool Image::saveImage()
		{
			if(isReadOnly())
				throw ImageIsReadOnlyException();
			
			return FreeImage_Save(_fif, _image, _path, 0);
		}
	}
}