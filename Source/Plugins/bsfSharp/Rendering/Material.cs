﻿//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#if IS_B3D
namespace BansheeEngine
#else
namespace bs
#endif
{
    /** @addtogroup Rendering
     *  @{
     */

    public partial class Material
    {
        /// <summary>
        /// Returns a texture assigned to the material.
        /// </summary>
        /// <param name="name">Name of the texture parameter.</param>
        /// <returns>Texture assigned to the specified material</returns>
        public RRef<Texture> GetTexture(string name)
        {
            return Internal_getTexture(mCachedPtr, name);
        }

        /// <summary>
        /// Assigns a texture to the specified material parameter. Allows you to specify a surface parameter that allows you
        /// to bind only a sub-set of the texture.
        /// </summary>
        /// <param name="name">Name of the texture parameter.</param>
        /// <param name="texture">Texture resource to assign.</param>
        /// <param name="surface">Subset of the texture to assign.</param>
        public void SetTexture(string name, RRef<Texture> texture, TextureSurface surface)
        {
            Internal_setTexture(mCachedPtr, name, texture, surface.mipLevel, surface.numMipLevels, surface.face, 
                surface.numFaces);
        }

        /// <summary>
        /// Assigns a texture to the specified material parameter.
        /// </summary>
        /// <param name="name">Name of the texture parameter.</param>
        /// <param name="texture">Texture resource to assign.</param>
        public void SetTexture(string name, RRef<Texture> texture)
        {
            Internal_setTexture(mCachedPtr, name, texture, 0, 0, 0, 0);
        }
    }

    /** @} */
}
