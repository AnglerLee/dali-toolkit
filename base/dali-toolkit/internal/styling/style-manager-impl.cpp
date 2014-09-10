/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// CLASS HEADER
#include "style-manager-impl.h"

// INTERNAL INCLUDES
#include <dali-toolkit/public-api/controls/control.h>
#include <dali-toolkit/public-api/controls/control-impl.h>
#include <dali-toolkit/public-api/styling/style-manager.h>
#include <dali-toolkit/internal/styling/util.h>
#include <dali/integration-api/debug.h>

// EXTERNAL INCLUDES
#include <fstream>
#include <iostream>
#include <sstream>


namespace
{

const char* LANDSCAPE_QUALIFIER = "landscape";
const char* PORTRAIT_QUALIFIER  = "portrait";

const char* DEFAULT_THEME = DALI_STYLE_DIR "tizen-default-theme.json";

const char* PACKAGE_PATH_KEY = "PACKAGE_PATH";
const char* DEFAULT_PACKAGE_PATH = DALI_DATA_READ_ONLY_DIR "/toolkit/";

} // namespace

namespace Dali
{

namespace Toolkit
{

namespace Internal
{

namespace
{

BaseHandle Create()
{
  BaseHandle handle = StyleManager::Get();

  if ( !handle )
  {
    SingletonService singletonService( SingletonService::Get() );
    if ( singletonService )
    {
      Toolkit::StyleManager manager = Toolkit::StyleManager( new Internal::StyleManager() );
      singletonService.Register( typeid( manager ), manager );
      handle = manager;
    }
  }

  return handle;
}
TypeRegistration STYLE_MANAGER_TYPE( typeid(Dali::Toolkit::StyleManager), typeid(Dali::BaseHandle), Create, true /* Create instance at startup */ );

/**
 * Merge two maps into one
 */
void MergeMaps( const PropertyValueMap& a, const PropertyValueMap& b, PropertyValueMap& out )
{
  out = a;
  for( PropertyValueMap::const_iterator it = b.begin(), itEnd = b.end(); it != itEnd; ++it )
  {
    out[ it->first ] = it->second;
  }
}

} // namespace

Toolkit::StyleManager StyleManager::Get()
{
  Toolkit::StyleManager manager;

  SingletonService singletonService( SingletonService::Get() );
  if ( singletonService )
  {
    // Check whether the style manager is already created
    Dali::BaseHandle handle = singletonService.GetSingleton( typeid( Toolkit::StyleManager ) );
    if( handle )
    {
      // If so, downcast the handle of singleton
      manager = Toolkit::StyleManager( dynamic_cast< StyleManager* >( handle.GetObjectPtr() ) );
    }
  }

  return manager;
}

StyleManager::StyleManager()
  : mOrientationDegrees( 0 ),  // Portrait
    mSetThemeConnection( false )
{
  // Add theme builder constants
  mThemeBuilderConstants[ PACKAGE_PATH_KEY ] = DEFAULT_PACKAGE_PATH;

  RequestDefaultTheme();

  if( Adaptor::IsAvailable() )
  {
    StyleMonitor::Get().StyleChangeSignal().Connect( this, &StyleManager::StyleMonitorChange );
  }
}

StyleManager::~StyleManager()
{
  // Disconnect from signal
  SetOrientation( Orientation() );
}

void StyleManager::SetOrientationValue( int orientation )
{
  mOrientationDegrees = orientation;

  Util::ConnectEventProcessingFinishedSignal();
  mSetThemeConnection = true;
}

int StyleManager::GetOrientationValue()
{
  return mOrientationDegrees;
}

void StyleManager::SetOrientation( Orientation orientation )
{
  if( mOrientation )
  {
    mOrientation.ChangedSignal().Disconnect( this, &StyleManager::OnOrientationChanged );
  }

  OnOrientationChanged( orientation );

  if( mOrientation )
  {
    mOrientation.ChangedSignal().Connect( this, &StyleManager::OnOrientationChanged );
  }
}

Orientation StyleManager::GetOrientation()
{
  return mOrientation;
}

void StyleManager::SetStyleConstant( const std::string& key, const Property::Value& value )
{
  mStyleBuilderConstants[ key ] = value;
}

bool StyleManager::GetStyleConstant( const std::string& key, Property::Value& valueOut )
{
  Toolkit::PropertyValueMap::iterator valueIt = mStyleBuilderConstants.find( key );
  if( valueIt != mStyleBuilderConstants.end() )
  {
    valueOut = valueIt->second;
    return true;
  }

  return false;
}

void StyleManager::OnOrientationChanged( Orientation orientation )
{
  mOrientation = orientation;

  if( mOrientation )
  {
    Util::ConnectEventProcessingFinishedSignal();
    mSetThemeConnection = true;
  }
}

Toolkit::Builder StyleManager::CreateBuilder( const PropertyValueMap& constants )
{
  Toolkit::Builder builder = Toolkit::Builder::New();
  builder.AddConstants( constants );

  return builder;
}

bool StyleManager::LoadJSON( Toolkit::Builder builder, const std::string& jsonFilePath )
{
  std::string fileString;
  if( LoadFile( jsonFilePath, fileString ) )
  {
    builder.LoadFromString( fileString );
    return true;
  }
  else
  {
    DALI_LOG_WARNING("Error loading file '%s'\n", jsonFilePath.c_str());
    return false;
  }
}

void StyleManager::CollectQualifiers( StringList& qualifiersOut )
{
  // Append the relevant qualifier for orientation
  int orientation = mOrientationDegrees;

  if( mOrientation )
  {
    orientation = mOrientation.GetDegrees();
  }

  switch( orientation )
  {
    case 90:
    case 270:
    {
      qualifiersOut.push_back( std::string( LANDSCAPE_QUALIFIER ) );
      break;
    }
    case 180:
    case 0: // fall through
    default:
    {
      qualifiersOut.push_back( std::string( PORTRAIT_QUALIFIER ) );
      break;
    }
  }
}

void StyleManager::BuildQualifiedStyleName( const std::string& styleName, const StringList& qualifiers, std::string& qualifiedStyleOut )
{
  qualifiedStyleOut.append( styleName );

  for( StringList::const_iterator it = qualifiers.begin(), itEnd = qualifiers.end(); it != itEnd; ++it )
  {
    const std::string& str = *it;

    qualifiedStyleOut.append( "-" );
    qualifiedStyleOut.append( str );
  }
}

void StyleManager::ApplyStyle( Toolkit::Builder builder, Toolkit::Control control )
{
  // Convert control name to lower case
  std::string styleName = control.GetTypeName();
  std::transform( styleName.begin(), styleName.end(), styleName.begin(), ::tolower );

  // Apply the style after choosing the correct actual style (e.g. landscape or portrait)
  StringList qualifiers;
  CollectQualifiers( qualifiers );

  while( true )
  {
    std::string qualifiedStyleName;
    BuildQualifiedStyleName( styleName, qualifiers, qualifiedStyleName );

    // Break if style found or we have tried the root style name (qualifiers is empty)
    if( builder.ApplyStyle( qualifiedStyleName, control ) || qualifiers.size() == 0 )
    {
      break;
    }

    // Remove the last qualifier in an attempt to find a style that is valid
    qualifiers.pop_back();
  }
}

void StyleManager::ApplyThemeStyle( Toolkit::Control control )
{
  if( mThemeBuilder )
  {
    ApplyStyle( mThemeBuilder, control );
  }
}

void StyleManager::ApplyStyle( Toolkit::Control control, const std::string& jsonFileName, const std::string& styleName )
{
  bool builderReady = false;

  // First look in the cache
  Toolkit::Builder builder = FindCachedBuilder( jsonFileName );
  if( builder )
  {
    builderReady = true;
  }
  else
  {
    // Merge theme and style constants
    PropertyValueMap constants;
    MergeMaps( mThemeBuilderConstants, mStyleBuilderConstants, constants );

    // Create it
    builder = CreateBuilder( constants );

    if( LoadJSON( builder, jsonFileName ) )
    {
      CacheBuilder( builder, jsonFileName );
      builderReady = true;
    }
  }

  // Apply the style to the control
  if( builderReady )
  {
    builder.ApplyStyle( styleName, control );
  }
}

bool StyleManager::LoadFile( const std::string& filename, std::string& stringOut )
{
  DALI_ASSERT_DEBUG( 0 != filename.length());

  std::ifstream in( filename.c_str(), std::ios::in );
  if( in )
  {
    std::stringstream buffer;
    buffer << in.rdbuf();

    stringOut = buffer.str();

    in.close();

    return true;
  }

  return false;
}

Toolkit::StyleManager::StyleChangeSignalType& StyleManager::StyleChangeSignal()
{
  return mStyleChangeSignal;
}

void StyleManager::RequestThemeChange( const std::string& themeFile )
{
  mThemeFile = themeFile;

  Util::ConnectEventProcessingFinishedSignal();
  mSetThemeConnection = true;
}

void StyleManager::RequestDefaultTheme()
{
  RequestThemeChange( DEFAULT_THEME );
}

bool StyleManager::IsThemeRequestPending()
{
  return mSetThemeConnection;
}

void StyleManager::SetTheme()
{
  mThemeBuilder = CreateBuilder( mThemeBuilderConstants );
  LoadJSON( mThemeBuilder, mThemeFile );

  mSetThemeConnection = false;

  StyleChange change;
  change.themeChange = true;
  mStyleChangeSignal.Emit( Toolkit::StyleManager::Get(), change );
}

Toolkit::Builder StyleManager::FindCachedBuilder( const std::string& key )
{
  BuilderMap::iterator builderIt = mBuilderCache.find( key );
  if( builderIt != mBuilderCache.end() )
  {
    return builderIt->second;
  }

  return Toolkit::Builder();
}

void StyleManager::CacheBuilder( Toolkit::Builder builder, const std::string& key )
{
  mBuilderCache[ key ] = builder;
}

void StyleManager::StyleMonitorChange( StyleMonitor styleMonitor, StyleChange styleChange )
{
  mStyleChangeSignal.Emit( Toolkit::StyleManager::Get(), styleChange );
}

} // namespace Internal

} // namespace Toolkit

} // namespace Dali
