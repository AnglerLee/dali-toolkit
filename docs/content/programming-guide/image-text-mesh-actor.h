/*! \page image-text-mesh-actor Image, Text and Mesh actors
 *
 *
 * <h1 class="pg">Overview</h1>
 * The Dali::ImageActor, Dali::TextActor, Dali::MeshActor are inherited from Dali::Actor and provide means to display resources like Images, Text and Geometries (Triangle meshes) on the stage.
 * All the Dali::Actor methods can be called on them.<br>
 *
 * - <b>ImageActor:</b> An actor for displaying Images. It allows the developer to display a Dali::Image object on the stage.<br>
 * - <b>TextActor:</b>  An actor for displaying text.<br>
 * - <b>MeshActor:</b>  An actor for displaying one or more mesh geometries. It may have children, which may be plain actors or other mesh actors.<br>
 *
 * <h1 class="pg">Image Actor</h1>
 *
 * <h2 class="pg">Construction</h2>
 * The Image Actor is constructed by passing a Dali::Image object
 *
 * @code
 * Dali::Image image = Image::New(myImageFilename);
 * Dali::ImageActor myImageActor = ImageActor::New(image);
 * @endcode
 *
 *
 * <h2 class="pg">Style</h2>
 * The Actor can render an image in two different ways.<br>
 * -# STYLE_QUAD: A simple flat quad style for redering image.<br>
 * -# STYLE_NINE_PATCH: This style gives the flexibility to stretch images by dividing it into 9 sections.
 * The four corners are unscaled; the four edges are scaled in one axis, and the middle is scaled in both axes.<br>
 *
 * @code
 * // default : ImageActor::STYLE_QUAD
 * myImageActor.SetStyle (Dali::ImageActor::STYLE_NINE_PATCH);
 * @endcode
 *
 *
 * <h2 class="pg">Border</h2>
 * The border is used in the ImageActor::STYLE_NINE_PATCH. It defines the border values of the image for stretching.<br>
 *
 * @code
 * Dali::ImageActor::Border border(0.45,0.15,0.45,0.15);
 * myImageActor.SetBorder(border);
 * @endcode
 *
 *
 * <h2 class="pg">Pixel area</h2>
 * The area of the image to be displayed by the Image Actor can be set by setting the Pixel area. Pixel area is relative to the top-left (0,0) of the image.
 * @code
 * Rect<int> pixel1( myX, myY, myWidth, myHeight );
 * if(!myImageActor.IsPixelAreaSet())
 * {
 *   myImageActor.SetPixelArea( pixel1 );
 * }
 *
 * //Removes the pixel are set
 * myImageActor.ClearPixelArea();
 * @endcode
 *
 *
 * <h2 class="pg">Changing the image</h2>
 * The Image Actor needs a reference to a Dali::Image object on creation. However the Image object can be later changed by calling DaliActor:SetImage
 * @code
 * myImageActor.SetImage( newImage );
 * @endcode
 *
 * <h2 class="pg">Fade in</h2>
 * It's possible to fade in the image gradually when first rendered.
 * @code
 * if (!myImageActor.GetFadeIn())
 * {
 *   myImageActor.SetFadeIn(true);
 * }
 *
 * // default : 1 Second
 * myImageActor.SetFadeInDuration(seconds);
 * @endcode
 *
 *
 *
 *
 *
 * <h1 class="pg">Text Actor</h1>
 *
 *
 * <h2 class="pg">Displaying Text</h2>
 * The text displayed by the text actor is initialised/set on construction, which can be changed later.
 *
 * @code
 * Dali::TextActor myTextActor = Dali::TextActor::New("Hi");
 * std::string str("Hello");
 * if (myTextActor.GetText() != str)
 * {
 *   myTextActor.SetText(str);
 * }
 * @endcode
 *
 *
 * <h2 class="pg">Fonts</h2>
 * It's possible to specify a font for the text displayed by the text actor.
 * @code
 * Dali::Font freeSerif = Dali::Font::New("FreeSerif", 8);
 * myTextActor.SetFont(freeSerif);
 * @endcode
 *
 *
 * <h2 class="pg">Ellipsis</h2>
 * It is possible to display an ellipsis in the TextActor when the text is truncated.
 * @code
 * std::string str("...");
 * if (myTextActor.GetEllipsis() != str)
 * {
 *   myTextActor.SetEllipsis(str);
 * }
 * @endcode
 *
 * <h2 class="pg">Style</h2>
 *
 * By calling the Dali::TextActor::SetTextStyle or by passing a Dali::TextStyle to the constructor, it's possible to define styling parameters such as color, font, size, outline, glow, shadow, italics or bold.
 * @code
 * TextStyle style;
 * style.SetItalic( true );
 *
 * myTextActor.SetTextStyle( style );
 * @endcode
 *
 * @see Dali::TextActor::SetTextStyle()
 *
 * It is possible to specify the text fit style for the text actor. The developer can specify whether the ellipsis should appear on the left, centre, or at the end
 * @code
 * // default : NONE
 * myTextActor.SetTextFitStyle(TextUtilities::EllipsizeRight);
 * @endcode
 *
 * <h2 class="pg">Loading state</h2>
 * It is possible to get the font loading status for the text and do processing accordingly.
 * @code
 * // observe text loading and do some processing when it's done
 * if( Dali::ResourceLoadingSucceeded == myTextActor.GetLoadingState() )
 * {
 *   // text already loaded, Do the processing here
 *   OnTextFontLoaded();
 * }
 * else
 * {
 *    // text not yet loaded, Connect to the SignalTextAvailable signal and Do the processing when it occurs
 *   myTextActor.SignalTextAvailable().Connect(this, &MyClass::OnTextFontLoaded);
 * }
 * @endcode
 *
 *
 *
 *
 * <h1 class="pg">Mesh Actor</h1>
 *
 * <h2 class="pg">Construction</h2>
 * The mesh actor is created by passing a reference to Dali::Mesh object
 *
 * @code
 * Dali::Mesh mesh = Dali::Mesh::New();
 * Dali::MeshActor myMeshActor = Dali::MeshActor::New(mesh);
 * @endcode

 *
 * <h2 class="pg">Modifying material</h2>
 * The developer can change the material of mesh actor using the material entity name.
 *
 * @code
 * Dali::Image image = Dali::Image::New(myTextureFile);
 * myCustomMaterial = Dali::Material::New("CustomMaterial");
 * myCustomMaterial.SetDiffuseTexture(image);
 * Dali::MeshActor::SetMaterial(myMeshActor, materialEntityNameInModel, 0, myCustomMaterial);
 *
 * @endcode
 *
 *
 */

