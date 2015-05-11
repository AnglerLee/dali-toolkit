/**
 *

# Text Label {#text-label}

## Overview

The Dali::Toolkit::TextLabel is a Dali::Toolkit::Control which renders a short text string.  
Text labels are lightweight, non-editable and do not respond to user input.

### Basic usage

To display a TextLabel the TEXT property must be set using a UTF-8 string.

~~~{.cpp}
// C++

TextLabel label = TextLabel::New();
label.SetProperty( TextLabel::Property::TEXT, "Hello World" );
label.SetAnchorPoint( AnchorPoint::TOP_LEFT );
Stage::GetCurrent().Add( label );
~~~

~~~{.js}
// JavaScript

var label = new dali.TextLabel();

label.text = "Hello World";
label.anchorPoint = dali.TOP_LEFT;

dali.stage.add( label );
~~~

The label must also be added to the stage, or to an actor which is on the stage.  
The position of the label on-screen is dependent on the parent-origin and anchor-point properties.  

|  |  |
|--|--|
| (ParentOrigin::TOP_LEFT, AnchorPoint::TOP_LEFT) | ![ ](../assets/img/text-controls/TextLabelTopLeft.png) ![ ](TextLabelTopLeft.png)   |

### Font Selection

By default TextLabel will automatically select a suitable font from the platform.  
Typically fonts do not support all scripts, for example Latin fonts often do not provide Arabic glyphs.  
Therefore you should expect TextLabel to select different fonts for each script.

Alternatively a font may be requested using either or all of FONT_FAMILY, FONT_STYLE, and POINT_SIZE properties:

~~~{.cpp}
// C++

label.SetProperty( TextLabel::Property::FONT_FAMILY, "HelveticaNue" );
label.SetProperty( TextLabel::Property::FONT_STYLE,  "Regular" );
label.SetProperty( TextLabel::Property::POINT_SIZE,  12.0f );
~~~

~~~{.js}
// JavaScript

label.fontFamily = "HelveticaNue";
label.fontStyle = "Regular";
label.pointSize = 12;
~~~

However the TextLabel will fall-back to using the default font, if the requested font does not support the required scripts.

### Font Styles

Setting a font size programmatically is not ideal for applications which support multiple screen resolutions etc.  
A more flexible approach is to prepare various JSON stylesheets, and request a different style for each platform:  

~~~{.cpp}
// C++
StyleManager styleManager = StyleManager::Get();
styleManager.RequestThemeChange( "example-path/example.json" );
~~~

To change the font for standard text labels, this JSON syntax can be used:

~~~{.json}
{
  "styles":
  {
    "textlabel":
    {
      "font-family":"Arial",
      "font-style":"Regular",
      "point-size":8
    }
  }
}
~~~

However the same point-size is unlikely to be suitable for all labels in an application.  
To set custom sizes simply set a "style name" for each case, and then provide a style override in JSON:

~~~{.cpp}
  // C++

  label.SetProperty( Control::Property::STYLE_NAME, "custom" );
~~~
~~~{.js}
  // JavaScript

  label.styleName = "custom"';
~~~

~~~{.json}
{
  "styles":
  {
    "textlabel":
    {
      "font-family":"Arial",
      "font-style":"Regular",
      "point-size":8
    },

    "custom":
    {
      "point-size":10
    }
  }
}
~~~

In the example above, standard text labels will have point-size 8, and "custom" labels will have point-size 10.  

### Text Alignment

Wrapping can be enabled using the MULTI_LINE property:

~~~{.cpp}
// C++

label.SetProperty( TextLabel::Property::MULTI_LINE, true );
~~~

~~~{.js}
// JavaScript

label.mutliLine = true;
~~~

The text can be either aligned horizontally to the beginning, end, or center of the available area:

~~~{.cpp}
// C++

label.SetProperty( TextLabel::Property::HORIZONTAL_ALIGNMENT, "BEGIN" ); // "CENTER" or "END"
~~~

~~~{.js}
// JavaScript

label.HorizontalAlignment = "BEGIN"; // "CENTER" or "END"
~~~

|  |  |
|--|--|
| Here is the "BEGIN" alignment shown for left-to-right (Latin)   |  right-to-left (Arabic) scripts |
| ![ ](../assets/img/text-controls/LatinBegin.png) ![ ](LatinBegin.png) | ![ ](../assets/img/text-controls/ArabicBegin.png) ![ ](ArabicBegin.png) |
| Here is the "CENTER" alignment shown for left-to-right (Latin)  | right-to-left (Arabic) scripts:|
| ![ ](../assets/img/text-controls/LatinCenter.png) ![ ](LatinCenter.png) | ![ ](../assets/img/text-controls/ArabicCenter.png) ![ ](ArabicCenter.png) |
| Here is the "END" alignment shown for left-to-right (Latin)  | right-to-left (Arabic) scripts:|
| ![ ](../assets/img/text-controls/LatinEnd.png) ![ ](LatinEnd.png) | ![ ](../assets/img/text-controls/ArabicEnd.png) ![ ](ArabicEnd.png) |


The examples above assume that the TextLabel size greater than the minimum required.  
The next section provides details about the other size related options.

## Negotiating size

\link size-negotiation Size negotiation \endlink is a layouting feature supported by UI controls such as TextLabel.
  
There are several resize policies which are commonly used with TextLabels.
  
The following examples show TextLabels actual size by setting a colored background, whilst the black area represents the size of the parent control:  

### Using natural size

With a "natural" size TextLabel will be large enough to display the text without wrapping, and will not have extra space to align the text within.  
Therefore in this example the same result would be displayed, regardless of the alignment or multi-line properties.  

~~~{.cpp}
// C++

TextLabel label = TextLabel::New( "Hello World" );
label.SetAnchorPoint( AnchorPoint::TOP_LEFT );
label.SetResizePolicy( ResizePolicy::USE_NATURAL_SIZE, Dimension::ALL_DIMENSIONS );
label.SetBackgroundColor( Color::BLUE );
Stage::GetCurrent().Add( label );
~~~

~~~{.js}
// JavaScript

var label = new dali.Textlabel;
label.text = "Hello World";
label.anchorPoint = dali.TOP_LEFT;

label.widthResizePolicy = "USE_NATURAL_SIZE";
label.heightResizePolicy = "USE_NATURAL_SIZE";

label.textColor = dali.COLOR_WHITE;
// background color not available as it's currently not a property of control
dali.stage.add( label );
~~~


 ![ ](../assets/img/text-controls/HelloWorld-NaturalSize.png)
 ![ ](HelloWorld-NaturalSize.png)


### Height-for-width negotiation

To layout text labels vertically, a fixed (maximum) width should be provided by the parent control.  
Each TextLabel will then report a desired height for the given width.  
Here is an example of this behavior using TableView as the parent:

~~~{.cpp}
// C++
TableView parent = TableView::New( 3, 1 );
parent.SetResizePolicy( ResizePolicy::FILL_TO_PARENT, Dimension::WIDTH );
parent.SetResizePolicy( ResizePolicy::USE_NATURAL_SIZE, Dimension::HEIGHT );
parent.SetAnchorPoint( AnchorPoint::TOP_LEFT );
Stage::GetCurrent().Add( parent );

TextLabel label = TextLabel::New( "Hello World" );
label.SetAnchorPoint( AnchorPoint::TOP_LEFT );
label.SetResizePolicy( ResizePolicy::FILL_TO_PARENT, Dimension::WIDTH );
label.SetResizePolicy( ResizePolicy::DIMENSION_DEPENDENCY, Dimension::HEIGHT );
label.SetBackgroundColor( Color::BLUE );
parent.AddChild( label, TableView::CellPosition( 0, 0 ) );
parent.SetFitHeight( 0 );

label = TextLabel::New( "A Quick Brown Fox Jumps Over The Lazy Dog" );
label.SetAnchorPoint( AnchorPoint::TOP_LEFT );
label.SetResizePolicy( ResizePolicy::FILL_TO_PARENT, Dimension::WIDTH );
label.SetResizePolicy( ResizePolicy::DIMENSION_DEPENDENCY, Dimension::HEIGHT );
label.SetBackgroundColor( Color::GREEN );
label.SetProperty( TextLabel::Property::MULTI_LINE, true );
parent.AddChild( label, TableView::CellPosition( 1, 0 ) );
parent.SetFitHeight( 1 );

label = TextLabel::New( "لإعادة ترتيب الشاشات، يجب تغيير نوع العرض إلى شبكة قابلة للتخصيص." );
label.SetAnchorPoint( AnchorPoint::TOP_LEFT );
label.SetResizePolicy( ResizePolicy::FILL_TO_PARENT, Dimension::WIDTH );
label.SetResizePolicy( ResizePolicy::DIMENSION_DEPENDENCY, Dimension::HEIGHT );
label.SetBackgroundColor( Color::BLUE );
label.SetProperty( TextLabel::Property::MULTI_LINE, true );
parent.AddChild( label, TableView::CellPosition( 2, 0 ) );
parent.SetFitHeight( 2 );
~~~

 ![ ](../assets/img/text-controls/HelloWorld-HeightForWidth.png)
 ![ ](HelloWorld-HeightForWidth.png)


Note that the "Hello World" text label (above) has been given the full width, not the natural width.

### TextLabel Decorations

#### Color

To change the color of the text, the recommended way is to use the TEXT_COLOR property.  
Note that unlike the Actor::COLOR property, this will not affect child Actors added to the TextLabel.  

~~~{.cpp}
// C++
label.SetProperty( TextLabel::Property::TEXT, "Red Text" );
label.SetProperty( TextLabel::Property::TEXT_COLOR, Color::RED );
~~~

~~~{.js}
// JavaScript

label.text = "Red Text";
label.textColor = dali.COLOR_RED;
~~~

 ![ ](../assets/img/text-controls/RedText.png)
 ![ ](RedText.png)

#### Drop Shadow

To add a drop-shadow to the text, simply set the SHADOW_OFFSET property with non-zero values.  
The color can also be selected using the SHADOW_COLOR property.  

~~~{.cpp}
 // C++

stage.SetBackgroundColor( Color::BLUE );

label1.SetProperty( TextLabel::Property::TEXT, "Plain Text" );

label2.SetProperty( TextLabel::Property::TEXT, "Text with Shadow" );
label2.SetProperty( TextLabel::Property::SHADOW_OFFSET, Vector2( 1.0f, 1.0f ) );
label2.SetProperty( TextLabel::Property::SHADOW_COLOR, Color::BLACK );

label3.SetProperty( TextLabel::Property::TEXT, "Text with Bigger Shadow" );
label3.SetProperty( TextLabel::Property::SHADOW_OFFSET, Vector2( 2.0f, 2.0f ) );
label3.SetProperty( TextLabel::Property::SHADOW_COLOR, Color::BLACK );

label4.SetProperty( TextLabel::Property::TEXT, "Text with Color Shadow" );
label4.SetProperty( TextLabel::Property::SHADOW_OFFSET, Vector2( 1.0f, 1.0f ) );
label4.SetProperty( TextLabel::Property::SHADOW_COLOR, Color::RED );
~~~

~~~{.js}
// JavaScript

dali.stage.setBackgroundColor( dali.COLOR_BLUE );

label1.text = "Plain Text";


label2.text = "Text with Shadow";
label2.shadowOffset = [1, 1];
label2.shadowColor = dali.COLOR_BLACK;

label3.text = "Text with Bigger Shadow";
label3.shadowOffset = [2, 2];
label3.shadowColor = dali.COLOR_BLACK;

label4.SetProperty( TextLabel::Property::TEXT, "Text with Color Shadow" );
label3.shadowOffset = [1, 1];
label3.shadowColor = dali.COLOR_RED;
~~~


![ ](../assets/img/text-controls/PlainText.png)
![ ](PlainText.png)


![ ](../assets/img/text-controls/TextWithShadow.png)
![ ](TextWithShadow.png)

![ ](../assets/img/text-controls/TextWithBiggerShadow.png)
![ ](TextWithBiggerShadow.png)


![ ](../assets/img/text-controls/TextWithColorShadow.png)
![ ](TextWithColorShadow.png)


#### Underline

The text can be underlined by setting UNDERLINE_ENABLED.  
The color can also be selected using the UNDERLINE_COLOR property.  

~~~{.cpp}
// C++
label1.SetProperty( TextLabel::Property::TEXT, "Text with Underline" );
label1.SetProperty( TextLabel::Property::UNDERLINE_ENABLED, true );

label2.SetProperty( TextLabel::Property::TEXT, "Text with Color Underline" );
label2.SetProperty( TextLabel::Property::UNDERLINE_ENABLED, true );
label2.SetProperty( TextLabel::Property::UNDERLINE_COLOR, Color::GREEN );
~~~
~~~{.js}
// JavaScript
label1.Text = "Text with Underline";
label1.underlineEnabled = true;

label2.Text = "Text with Color Underline";
label2.underlineEnabled = true;
label2.underlineColor = dali.COLOR_GREEN;
~~~


![ ](../assets/img/text-controls/TextWithUnderline.png)
![ ](TextWithUnderline.png)


![ ](../assets/img/text-controls/TextWithColorUnderline.png)
![ ](TextWithColorUnderline.png)

By default the underline height will be taken from the font metrics, however this can be overridden using the UNDERLINE_HEIGHT property:

~~~{.cpp}
// C++

label1.SetProperty( TextLabel::Property::UNDERLINE_HEIGHT, 1.0f );
~~~

~~~{.js}
// JavaScript

label1.underlineHeight = 1;
~~~

![ ](../assets/img/text-controls/TextWith1pxUnderline.png)
![ ](TextWith1pxUnderline.png)

### Text Label Properties

 Name (JavaScript)   |  Name (C++)         |  Type        | Writable     | Animatable
---------------------|---------------------|--------------|--------------|-----------
 renderingBackend    | RENDERING_BACKEND   |  INTEGER     | &#10004;     | &#10008;
 text                | TEXT                |  STRING      | &#10004;     | &#10008;
 fontFamily          | FONT_FAMILY         |  STRING      | &#10004;     | &#10008;
 fontStyle           | FONT_STYLE          |  STRING      | &#10004;     | &#10008;
 pointSize           | POINT_SIZE          |  FLOAT       | &#10004;     | &#10008;
 multiLine           | MULTI_LINE          |  BOOLEAN     | &#10004;     | &#10008;
 horizontalAlignment | HORIZONTAL_ALIGNMENT|  STRING      | &#10004;     | &#10008;
 verticalAlignment   | VERTICAL_ALIGNMENT  |  STRING      | &#10004;     | &#10008;
 textColor           | TEXT_COLOR          |  VECTOR4     | &#10004;     | &#10008;
 shadowOffset        | SHADOW_OFFSET       |  VECTOR2     | &#10004;     | &#10008;
 shadowColor         | SHADOW_COLOR        |  VECTOR4     | &#10004;     | &#10008;
 underlineEnabled    | UNDERLINE_ENABLED   |  BOOLEAN     | &#10004;     | &#10008;
 underlineColor      | UNDERLINE_COLOR     |  VECTOR4     | &#10004;     | &#10008;
 underlineHeight     | UNDERLINE_HEIGHT    |  FLOAT       | &#10004;     | &#10008;



@class TextLabel

*/
