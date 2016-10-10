/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
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
 *
 */
#include <stdlib.h>
#include <unistd.h>
#include <dali/dali.h>
#include <dali-toolkit-test-suite-utils.h>
#include <toolkit-event-thread-callback.h>
#include <dali-toolkit/devel-api/image-loader/async-image-loader.h>

using namespace Dali;
using namespace Dali::Toolkit;

namespace
{
// resolution: 34*34, pixel format: RGBA8888
static const char* gImage_34_RGBA = TEST_RESOURCE_DIR "/icon-edit.png";
// resolution: 50*50, pixel format: RGBA8888
static const char* gImage_50_RGBA = TEST_RESOURCE_DIR "/icon-delete.png";
// resolution: 128*128, pixel format: RGB888
static const char* gImage_128_RGB = TEST_RESOURCE_DIR "/gallery-small-1.jpg";

// for testing the ImageLoadedSignal
class ImageLoadedSignalVerifier : public ConnectionTracker
{
public:

  ImageLoadedSignalVerifier()
  : mCount( 0 )
  {}

  virtual ~ImageLoadedSignalVerifier()
  {}

  void ImageLoaded( uint32_t id, PixelData pixelData )
  {
    mIDs.push_back( id );
    mPixelDataList.push_back( pixelData );
    mCount++;
  }

  int LoadedImageCount()
  {
    return mCount;
  }

  bool Verify( uint32_t id, uint32_t width, uint32_t height )
  {
    int size = mIDs.size();
    for( int i = 0; i<size; i++  )
    {
      if( mIDs[i] == id )
      {
        return mPixelDataList[i].GetWidth() == width
            && mPixelDataList[i].GetHeight() == height;
      }
    }

    return false;
  }

private:

  int mCount;

  std::vector<uint32_t> mIDs;
  std::vector<PixelData> mPixelDataList;
};


} // anonymous namespace

void dali_async_image_loader_startup(void)
{
  test_return_value = TET_UNDEF;
}

void dali_async_image_loader_cleanup(void)
{
  test_return_value = TET_PASS;
}

int UtcDaliImageAtlasNew01(void)
{
  ToolkitTestApplication application;

  //invoke default handle constructor
  AsyncImageLoader loader;

  DALI_TEST_CHECK( !loader );

  // initialise handle
  loader = AsyncImageLoader::New();
  DALI_TEST_CHECK( loader );

  END_TEST;
}

int UtcDaliAsyncImageLoaderCopyConstructor(void)
{
  ToolkitTestApplication application;

  AsyncImageLoader loader = AsyncImageLoader::New( );
  DALI_TEST_CHECK( loader );

  AsyncImageLoader loaderCopy(loader);
  DALI_TEST_CHECK( loaderCopy );

  END_TEST;
}

int UtcDaliAsyncImageLoaderAssignmentOperator(void)
{
  ToolkitTestApplication application;

  AsyncImageLoader loader = AsyncImageLoader::New();
  DALI_TEST_CHECK( loader );

  AsyncImageLoader loader2;
  DALI_TEST_CHECK( !loader2 );

  loader2 = loader;
  DALI_TEST_CHECK( loader2 );
  DALI_TEST_CHECK( loader == loader2 ); // the two handles are pointing to the same object.

  END_TEST;
}

int UtcDaliAsyncImageLoaderLoadAndLoadedSignal(void)
{
  ToolkitTestApplication application;

  AsyncImageLoader loader = AsyncImageLoader::New();
  ImageLoadedSignalVerifier loadedSignalVerifier;

  loader.ImageLoadedSignal().Connect( &loadedSignalVerifier, &ImageLoadedSignalVerifier::ImageLoaded );

  loader.Load( gImage_34_RGBA );
  uint32_t id02 = loader.Load( gImage_50_RGBA, ImageDimensions( 25, 25 ) );
  uint32_t id03 = loader.Load( gImage_128_RGB, ImageDimensions( 100, 100 ), FittingMode::SCALE_TO_FILL, SamplingMode::BOX_THEN_LINEAR, true );

  EventThreadCallback* eventTrigger = EventThreadCallback::Get();
  CallbackBase* callback = eventTrigger->GetCallback();

  eventTrigger->WaitingForTrigger( 3 );// waiting until all three images are loaded

  CallbackBase::Execute( *callback );

  application.SendNotification();
  application.Render();

  DALI_TEST_CHECK( loadedSignalVerifier.LoadedImageCount() == 3 );
  DALI_TEST_CHECK( loadedSignalVerifier.Verify( id02, 25, 25 ) );
  DALI_TEST_CHECK( loadedSignalVerifier.Verify( id03, 100, 100 ) );

  END_TEST;
}

int UtcDaliAsyncImageLoaderCancel(void)
{
  ToolkitTestApplication application;

  AsyncImageLoader loader = AsyncImageLoader::New();
  ImageLoadedSignalVerifier loadedSignalVerifier;

  loader.ImageLoadedSignal().Connect( &loadedSignalVerifier, &ImageLoadedSignalVerifier::ImageLoaded );

  uint32_t id01 = loader.Load( gImage_34_RGBA, ImageDimensions( 34, 34 ) );
  uint32_t id02 = loader.Load( gImage_50_RGBA, ImageDimensions( 25, 25 ) );
  uint32_t id03 = loader.Load( gImage_128_RGB, ImageDimensions( 100, 100 ), FittingMode::SCALE_TO_FILL, SamplingMode::BOX_THEN_LINEAR, true );

  // cancel the loading of the second image
  DALI_TEST_CHECK( loader.Cancel( id02 ) );

  EventThreadCallback* eventTrigger = EventThreadCallback::Get();
  CallbackBase* callback = eventTrigger->GetCallback();

  eventTrigger->WaitingForTrigger( 2 );// waiting until first and third images are loaded

  CallbackBase::Execute( *callback );

  DALI_TEST_CHECK( ! loader.Cancel( id03 ) ); // can not cancel a task that is already implemeted

  application.SendNotification();
  application.Render();

  DALI_TEST_CHECK( loadedSignalVerifier.LoadedImageCount() == 2 );

  DALI_TEST_CHECK( loadedSignalVerifier.Verify( id01, 34, 34 ) );  // first image is successfully loaded
  DALI_TEST_CHECK( !loadedSignalVerifier.Verify( id02, 25, 25 ) ); // second image is not loaded
  DALI_TEST_CHECK( loadedSignalVerifier.Verify( id03, 100, 100 ) ); // third image is successfully loaded

  END_TEST;
}

int UtcDaliAsyncImageLoaderCancelAll(void)
{
  ToolkitTestApplication application;

  AsyncImageLoader loader = AsyncImageLoader::New();
  ImageLoadedSignalVerifier loadedSignalVerifier;

  loader.ImageLoadedSignal().Connect( &loadedSignalVerifier, &ImageLoadedSignalVerifier::ImageLoaded );

  uint32_t id01 = loader.Load( gImage_34_RGBA, ImageDimensions( 34, 34 ) );
  uint32_t id02 = loader.Load( gImage_50_RGBA, ImageDimensions( 25, 25 ) );

  // cancel the loading of the first and second image
  loader.CancelAll();

  uint32_t id03 = loader.Load( gImage_128_RGB, ImageDimensions( 100, 100 ), FittingMode::SCALE_TO_FILL, SamplingMode::BOX_THEN_LINEAR, true );

  EventThreadCallback* eventTrigger = EventThreadCallback::Get();
  CallbackBase* callback = eventTrigger->GetCallback();

  eventTrigger->WaitingForTrigger( 1 );// waiting until the third images is loaded

  CallbackBase::Execute( *callback );

  application.SendNotification();
  application.Render();

  DALI_TEST_CHECK( loadedSignalVerifier.LoadedImageCount() == 1 );

  DALI_TEST_CHECK( !loadedSignalVerifier.Verify( id01, 34, 34 ) );  // first image is not loaded
  DALI_TEST_CHECK( !loadedSignalVerifier.Verify( id02, 25, 25 ) ); // second image is not loaded
  DALI_TEST_CHECK( loadedSignalVerifier.Verify( id03, 100, 100 ) ); // third image is successfully loaded

  END_TEST;
}


