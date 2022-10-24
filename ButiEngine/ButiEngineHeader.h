#pragma once

#ifndef ButiEngineHeader_H

#define ButiEngineHeader_H




#define WIN32_LEAN_AND_MEAN           
#pragma once
#include<cstdint>
const std::int32_t splitNum = 8;
#include<Windows.h>

#include <tchar.h>
#include <wrl.h>

#include <assert.h>
#include <wchar.h>
#include <mmsystem.h>
#include <atlbase.h>
#include <math.h>
#include<direct.h>

#include<chrono>

#include <vector>
#include <list>
#include <map>
#include<unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <exception>
#include <stdexcept>

#pragma warning(disable:4192)

#include"ButiMemorySystem/ButiMemorySystem/MemoryAllocator.h"
#include"ButiEngineHeader/Header/Device/GUI.h"
#include"ButiRendering_Dx12/ButiRendering/Common.h"

#include"ButiSound/ButiSound/ResourceInterface.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiList.h"
#include"Header/Common.h"
#include"Header/GameObjects/DefaultGameComponent/DefaultGameComponent.h"
#pragma warning(default:4192)


#endif // !ButiEngineHeader_H