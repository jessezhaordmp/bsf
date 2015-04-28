#include "BsGUITexture.h"
#include "BsImageSprite.h"
#include "BsGUIWidget.h"
#include "BsGUISkin.h"
#include "BsSpriteTexture.h"
#include "BsGUIDimensions.h"
#include "BsTexture.h"

namespace BansheeEngine
{
	const String& GUITexture::getGUITypeName()
	{
		static String name = "Texture";
		return name;
	}

	GUITexture::GUITexture(const String& styleName, const HSpriteTexture& texture, 
		GUIImageScaleMode scale, bool transparent, const GUIDimensions& dimensions)
		:GUIElement(styleName, dimensions), mScaleMode(scale), mUsingStyleTexture(false),
		mTransparent(transparent)
	{
		mImageSprite = bs_new<ImageSprite, PoolAlloc>();

		if(texture != nullptr)
		{
			mActiveTexture = texture;
			mUsingStyleTexture = false;
		}
		else
		{
			mActiveTexture = _getStyle()->normal.texture;
			mUsingStyleTexture = true;
		}
	}

	GUITexture::~GUITexture()
	{
		bs_delete<PoolAlloc>(mImageSprite);
	}

	GUITexture* GUITexture::create(const HSpriteTexture& texture, GUIImageScaleMode scale, bool transparent,
		const GUIOptions& options, const String& styleName)
	{
		return new (bs_alloc<GUITexture, PoolAlloc>()) GUITexture(getStyleName<GUITexture>(styleName),
			texture, scale, transparent, GUIDimensions::create(options));
	}

	GUITexture* GUITexture::create(const HSpriteTexture& texture, GUIImageScaleMode scale, bool transparent,
		const String& styleName)
	{
		return new (bs_alloc<GUITexture, PoolAlloc>()) GUITexture(getStyleName<GUITexture>(styleName),
			texture, scale, transparent, GUIDimensions::create());
	}

	GUITexture* GUITexture::create(const HSpriteTexture& texture, GUIImageScaleMode scale, 
		const GUIOptions& options, const String& styleName)
	{
		return new (bs_alloc<GUITexture, PoolAlloc>()) GUITexture(getStyleName<GUITexture>(styleName), 
			texture, scale, true, GUIDimensions::create(options));
	}

	GUITexture* GUITexture::create(const HSpriteTexture& texture, GUIImageScaleMode scale, 
		const String& styleName)
	{
		return new (bs_alloc<GUITexture, PoolAlloc>()) GUITexture(getStyleName<GUITexture>(styleName), 
			texture, scale, true, GUIDimensions::create());
	}

	GUITexture* GUITexture::create(const HSpriteTexture& texture, 
		const GUIOptions& options, const String& styleName)
	{
		return new (bs_alloc<GUITexture, PoolAlloc>()) GUITexture(getStyleName<GUITexture>(styleName), 
			texture, GUIImageScaleMode::StretchToFit, true, GUIDimensions::create(options));
	}

	GUITexture* GUITexture::create(const HSpriteTexture& texture, const String& styleName)
	{
		return new (bs_alloc<GUITexture, PoolAlloc>()) GUITexture(getStyleName<GUITexture>(styleName),
			texture, GUIImageScaleMode::StretchToFit, true, GUIDimensions::create());
	}

	GUITexture* GUITexture::create(GUIImageScaleMode scale, const GUIOptions& options, const String& styleName)
	{
		return new (bs_alloc<GUITexture, PoolAlloc>()) GUITexture(getStyleName<GUITexture>(styleName), 
			HSpriteTexture(), scale, true, GUIDimensions::create(options));
	}

	GUITexture* GUITexture::create(GUIImageScaleMode scale, const String& styleName)
	{
		return new (bs_alloc<GUITexture, PoolAlloc>()) GUITexture(getStyleName<GUITexture>(styleName), 
			HSpriteTexture(), scale, true, GUIDimensions::create());
	}

	GUITexture* GUITexture::create(const GUIOptions& options, const String& styleName)
	{
		return new (bs_alloc<GUITexture, PoolAlloc>()) GUITexture(getStyleName<GUITexture>(styleName), 
			HSpriteTexture(), GUIImageScaleMode::StretchToFit, true, GUIDimensions::create(options));
	}

	GUITexture* GUITexture::create(const String& styleName)
	{
		return new (bs_alloc<GUITexture, PoolAlloc>()) GUITexture(getStyleName<GUITexture>(styleName), 
			HSpriteTexture(), GUIImageScaleMode::StretchToFit, true, GUIDimensions::create());
	}

	void GUITexture::setTint(const Color& color)
	{
		mColor = color;

		markContentAsDirty();
	}

	void GUITexture::setTexture(const HSpriteTexture& texture)
	{
		mActiveTexture = texture;
		mUsingStyleTexture = false;

		markContentAsDirty();
	}

	UINT32 GUITexture::_getNumRenderElements() const
	{
		return mImageSprite->getNumRenderElements();
	}

	const GUIMaterialInfo& GUITexture::_getMaterial(UINT32 renderElementIdx) const
	{
		return mImageSprite->getMaterial(renderElementIdx);
	}

	UINT32 GUITexture::_getNumQuads(UINT32 renderElementIdx) const
	{
		return mImageSprite->getNumQuads(renderElementIdx);
	}

	void GUITexture::updateRenderElementsInternal()
	{		
		mDesc.width = mWidth;
		mDesc.height = mHeight;

		mDesc.borderLeft = _getStyle()->border.left;
		mDesc.borderRight = _getStyle()->border.right;
		mDesc.borderTop = _getStyle()->border.top;
		mDesc.borderBottom = _getStyle()->border.bottom;
		mDesc.transparent = mTransparent;
		mDesc.color = mColor;

		float optimalWidth = 0.0f;
		float optimalHeight = 0.0f;
		if (SpriteTexture::checkIsLoaded(mActiveTexture))
		{
			mDesc.texture = mActiveTexture.getInternalPtr();
			optimalWidth = (float)mDesc.texture->getWidth();
			optimalHeight = (float)mDesc.texture->getHeight();
		}

		switch (mScaleMode)
		{
		case GUIImageScaleMode::StretchToFit:
			mDesc.uvScale = Vector2(1.0f, 1.0f);
			break;
		case GUIImageScaleMode::ScaleToFit:
			mDesc.uvScale.x = optimalWidth / mWidth;
			mDesc.uvScale.y = optimalHeight / mHeight;

			if(mDesc.uvScale.x < mDesc.uvScale.y)
			{
				mDesc.uvScale.x = 1.0f;
				mDesc.uvScale.y = (mWidth * (optimalHeight / optimalWidth)) / mHeight;
			}
			else
			{
				mDesc.uvScale.x = (mHeight * (optimalWidth / optimalHeight)) / mWidth;
				mDesc.uvScale.y = 1.0f;
			}

			break;
		case GUIImageScaleMode::CropToFit:
			mDesc.uvScale.x = optimalWidth / mWidth;
			mDesc.uvScale.y = optimalHeight / mHeight;

			if(mDesc.uvScale.x < mDesc.uvScale.y)
			{
				mDesc.uvScale.x = (mHeight * (optimalWidth / optimalHeight)) / mWidth;
				mDesc.uvScale.y = 1.0f;
			}
			else
			{
				mDesc.uvScale.x = 1.0f;
				mDesc.uvScale.y = (mWidth * (optimalHeight / optimalWidth)) / mHeight;
			}

			break;
		case GUIImageScaleMode::RepeatToFit:
			mDesc.uvScale.x = mWidth / optimalWidth;
			mDesc.uvScale.y = mHeight / optimalHeight;
			break;
		default:
			break;
		}

		mImageSprite->update(mDesc, (UINT64)_getParentWidget());
		
		GUIElement::updateRenderElementsInternal();
	}

	void GUITexture::updateClippedBounds()
	{
		mClippedBounds = mImageSprite->getBounds(mOffset, mClipRect);
	}

	void GUITexture::styleUpdated()
	{
		if (mUsingStyleTexture)
			mActiveTexture = _getStyle()->normal.texture;
	}

	Vector2I GUITexture::_getOptimalSize() const
	{
		Vector2I optimalSize;

		if(_getDimensions().fixedWidth())
			optimalSize.x = _getDimensions().minWidth;
		else
		{
			if (SpriteTexture::checkIsLoaded(mActiveTexture))
				optimalSize.x = mActiveTexture->getWidth();
			else
				optimalSize.x = _getDimensions().maxWidth;
		}

		if(_getDimensions().fixedHeight())
			optimalSize.y = _getDimensions().minHeight;
		else
		{
			if (SpriteTexture::checkIsLoaded(mActiveTexture))
				optimalSize.y = mActiveTexture->getHeight();
			else
				optimalSize.y = _getDimensions().maxHeight;
		}

		return optimalSize;
	}

	void GUITexture::_fillBuffer(UINT8* vertices, UINT8* uv, UINT32* indices, UINT32 startingQuad, UINT32 maxNumQuads, 
		UINT32 vertexStride, UINT32 indexStride, UINT32 renderElementIdx) const
	{
		mImageSprite->fillBuffer(vertices, uv, indices, startingQuad, maxNumQuads, vertexStride, indexStride, renderElementIdx, mOffset, mClipRect);
	}
}