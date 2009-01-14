/* -*-c++-*- */
/* osgEarth - Dynamic map generation toolkit for OpenSceneGraph
 * Copyright 2008-2009 Pelican Ventures, Inc.
 * http://osgearth.org
 *
 * osgEarth is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include <osgEarth/ImageToHeightFieldConverter>

#include <osg/Notify>

using namespace osgEarth;

osg::HeightField* ImageToHeightFieldConverter::convert(osg::Image* image, float scaleFactor)
{
    //osg::notify(osg::NOTICE) << "Scale factor " << scaleFactor << std::endl;
	if (image)
	{
        osg::HeightField *hf = new osg::HeightField;
		//osg::notify(osg::NOTICE) << "Read heightfield image" << std::endl;
		hf->allocate(image->s(), image->t());
				
		for( unsigned int row=0; row < image->t(); row++ ) {
            //osg::notify(osg::NOTICE) << "ROW " << row << ":\t";
            for( unsigned int col=0; col < image->s(); col++ ) {
                unsigned char* ptr = image->data( col, row );
                if ( image->getPixelSizeInBits() == 16 ) {
                    short val = (short)*(short*)ptr;
                    hf->setHeight( col, row, ((float)val) * scaleFactor );
                    //osg::notify(osg::NOTICE) << val << "\t";
                }
                else if ( image->getPixelSizeInBits() == 32 ) {
                    float val = (float)*(float*)ptr;
                    hf->setHeight( col, row, val * scaleFactor );
                    //osg::notify(osg::NOTICE) << val << "\t";
                }
            }
            //osg::notify(osg::NOTICE) << std::endl;
        }
        return hf;
    }
    return NULL;
}

osg::Image* ImageToHeightFieldConverter::convert(osg::HeightField* hf, int pixelSize /*=16*/)
{
	if (hf)
	{
        int type;
        if (pixelSize == 16) type = GL_SHORT;
        else if (pixelSize == 32) type = GL_FLOAT;
        else type = GL_SHORT;

        osg::Image* image = new osg::Image;
        image->allocateImage(hf->getNumColumns(), hf->getNumRows(), 1, GL_LUMINANCE, type);

        for( unsigned int row=0; row < hf->getNumRows(); row++ ) {
            for( unsigned int col=0; col < hf->getNumColumns(); col++ ) {
                if (pixelSize == 16)
                {
                  short val = (short)hf->getHeight(col,row);
                  //osg::notify(osg::NOTICE) << "Short val " << val << std::endl;
                  *((short*)image->data( col, row)) = val;
                }
                else if (pixelSize == 32)
                {
                    float val = (float)hf->getHeight(col,row);
                    //osg::notify(osg::NOTICE) << "Float val " << val << std::endl;
                    *image->data( col, row) = val;
                }
            }
        }
        return image;
    }
    return NULL;
}