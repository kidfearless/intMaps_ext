/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod IntMap Extension
 * Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include "extension.h"
#include "IHandleSys.h"
#include "smsdk_ext.h"

#include <iostream>
#include <vector>
#include <string>
#include <map>

HandleType_t g_IntMapType = 0;

/**
 * @file extension.cpp
 * @brief Implement extension code here.
 */

IntMapHandler g_IntMap; /**< Global singleton for extension's main interface */

const sp_nativeinfo_t g_IntMapNatives[] =
{
	{"IntMap.IntMap",					Native_IntMapIntMap},

	{"IntMap.SetValue",					Native_IntMap_SetValue},
	{"IntMap.GetValue",					Native_IntMap_GetValue},

	{"IntMap.SetArray",					Native_IntMap_SetArray},
	{"IntMap.GetArray",					Native_IntMap_GetArray},
	{"IntMap.GetArrayCell",				Native_IntMap_GetArrayCell},
	{"IntMap.GetArrayLength",			Native_IntMap_GetArrayLength},


	{"IntMap.SetString",				Native_IntMap_SetString},
	{"IntMap.GetString",				Native_IntMap_GetString},
	{"IntMap.GetStringLength",			Native_IntMap_GetString},

	{"IntMap.RemoveCell",				Native_IntMap_RemoveCell},
	{"IntMap.RemoveString",				Native_IntMap_RemoveString},
	{"IntMap.RemoveArray",				Native_IntMap_RemoveArray},

	{"IntMap.RemoveAll",				Native_IntMap_RemoveAll},

	{"IntMap.ClearCells",				Native_IntMap_ClearCells},
	{"IntMap.ClearArrays",				Native_IntMap_ClearArrays},
	{"IntMap.ClearStrings",				Native_IntMap_ClearStrings},
	{"IntMap.ClearAll",					Native_IntMap_ClearAll},

	{"IntMap.HasValue",					Native_IntMap_HasCells},
	{"IntMap.HasArray",					Native_IntMap_HasArrays},
	{"IntMap.HasString",				Native_IntMap_HasString},


	{"IntMap.CellSize.get",				Native_IntMap_CellSizeGet},
	{"IntMap.StringSize.get",			Native_IntMap_StringSizeGet},
	{"IntMap.ArraySize.get",			Native_IntMap_ArraySizeGet},


	{"IntMap.IterateCells",				Native_IntMap_IterateCells},
	{"IntMap.IterateArrays",			Native_IntMap_IterateArrays},
	{"IntMap.IterateStrings",			Native_IntMap_IterateStrings},



	{nullptr,							nullptr},
};

bool IntMapHandler::SDK_OnLoad(char *error, size_t maxlen, bool late)
{
	HandleError err;
	g_IntMapType = handlesys->CreateType("IntMap", this, 0, nullptr, nullptr, myself->GetIdentity(), &err);

	if (g_IntMapType == 0)
	{
		snprintf(error, maxlen, "Could not create IntMap (err: %d)", err);
	}

	sharesys->AddNatives(myself, g_IntMapNatives);
	sharesys->RegisterLibrary(myself, "intmap_ext");

	// g_pSM->LogMessage(myself, "%s", buffer);

	return true;
}

void IntMapHandler::SDK_OnUnload()
{
	handlesys->RemoveType(g_IntMapType, myself->GetIdentity());
}

void IntMapHandler::OnHandleDestroy(HandleType_t type, void *object)
{
	if (type == g_IntMapType)
	{
		delete (IntMap*) object;
	}
}

// simplified function to create a handle for us and handle any errors.
cell_t IntMapHandler::CreateHandle(IPluginContext *const pContext)
{
	auto intmap = new IntMap();

	HandleError handleError;
	auto handle = handlesys->CreateHandle(g_IntMapType, intmap, pContext->GetIdentity(), myself->GetIdentity(), &handleError);

	if (handle == BAD_HANDLE)
	{
		delete intmap;
		return pContext->ThrowNativeError("Cannot create IntMap (err: %d)", handleError);
	}


	return static_cast<cell_t>(handle);
}

// simplified function to take a native and spit out an intmap
void IntMapHandler::ReadHandle(IPluginContext *const pContext, const cell_t *params, pIntMap_t *value)
{
	HandleSecurity security(pContext->GetIdentity(), myself->GetIdentity());

	auto error = handlesys->ReadHandle(static_cast<Handle_t>(params[1]), g_IntMapType, &security, reinterpret_cast<void**>(value));

	if (error != HandleError_None)
	{
		pContext->ThrowNativeError("Error with reading IntMap Handle (err: %d)", error);
	}
}

//	public native IntMap();
cell_t Native_IntMapIntMap(IPluginContext *pContext, const cell_t *params)
{
	return IntMapHandler::CreateHandle(pContext);
}

// 	public native void SetValue(const int key, any value);
cell_t Native_IntMap_SetValue(IPluginContext *pContext, const cell_t *params)
{
	IntMap* value;
	IntMapHandler::ReadHandle(pContext, params, &value);
	value->SetValue(params[2], params[3]);
	return 0;
}

// public native any  GetValue(const int key);
cell_t Native_IntMap_GetValue(IPluginContext *pContext, const cell_t *params)
{
	IntMap *value;
	IntMapHandler::ReadHandle(pContext, params, &value);

	return value->GetValue(params[2]);
}

// public native void SetArray(const int key, any[] value, int maxlength);
cell_t Native_IntMap_SetArray(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	cell_t key = params[2];
	cell_t localAddress = params[3];
	cell_t length = params[4];

	if (params[4] < 0)
	{
		return pContext->ThrowNativeError("Invalid array size: %d", params[4]);
	}

	cell_t* array;
	pContext->LocalToPhysAddr(params[3], &array);

	intmap->SetArray(params[2], array, params[4]);

	return 0;
}

// public native void GetArray(const int key, any[] value, int maxlength);
cell_t Native_IntMap_GetArray(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	cell_t key = params[2];
	cell_t localAddress = params[3];
	cell_t length = params[4];

	if (params[4] < 0)
	{
		return pContext->ThrowNativeError("Invalid array size: %d", params[4]);
	}

	cell_t *array;
	pContext->LocalToPhysAddr(params[3], &array);

	memcpy(array, intmap->GetArray(params[2]).data(), sizeof(cell_t) * intmap->GetArray(params[2]).size());

	return 0;
}

// public native any  GetArrayCell(const int key, int index);
cell_t Native_IntMap_GetArrayCell(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	cell_t key = params[2];
	cell_t index = params[3];
	return intmap->GetArrayCell(key, index);
}

// public native int  GetArrayLength(const int key);
cell_t Native_IntMap_GetArrayLength(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	return sizeof(cell_t) * intmap->GetArray(params[2]).size();
}

// public native void SetString(const int key, const char[] value);
cell_t Native_IntMap_SetString(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	char *paramString;
	pContext->LocalToString(params[3], &paramString);

	intmap->SetString(params[2], paramString);

	return 0;
}

// public native void GetString(const int key, char[] value, int maxlength);
cell_t Native_IntMap_GetString(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	std::string string = intmap->GetString(params[2]);

	char *paramString;
	pContext->LocalToString(params[3], &paramString);

	StringCopy(paramString, params[4], string.c_str());

	return 0;
}

// public native int  GetStringLength(const int key);
cell_t Native_IntMap_GetStringLength(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	return intmap->GetString(params[2]).size()+1;
}

// public native void RemoveCell(const int key);
cell_t Native_IntMap_RemoveCell(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	intmap->RemoveCell(params[2]);
	return 0;
}

// public native void RemoveString(const int key);
cell_t Native_IntMap_RemoveString(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	intmap->RemoveString(params[2]);
	return 0;
}

// public native void RemoveArray(const int key);
cell_t Native_IntMap_RemoveArray(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	intmap->RemoveArray(params[2]);
	return 0;
}

// public native void RemoveAll(const int key);
cell_t Native_IntMap_RemoveAll(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	intmap->RemoveAll(params[2]);
	return 0;
}

// public native void ClearCells();
cell_t Native_IntMap_ClearCells(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	intmap->ClearCells();

	return 0;
}

// public native void ClearArrays();
cell_t Native_IntMap_ClearArrays(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	intmap->ClearArrays();
	return 0;
}

// public native void ClearStrings();
cell_t Native_IntMap_ClearStrings(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	intmap->ClearStrings();
	return 0;
}

// public native void ClearAll();
cell_t Native_IntMap_ClearAll(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	intmap->ClearAll();
	return 0;
}

// public native bool HasValue(const int key);
cell_t Native_IntMap_HasCells(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	return intmap->HasValue(params[2]);
}

// public native bool HasArray(const int key);
cell_t Native_IntMap_HasArrays(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	return intmap->HasArray(params[2]);
}

// public native bool HasString(const int key);
cell_t Native_IntMap_HasString(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	return intmap->HasString(params[2]);
}

// property int CellSize
cell_t Native_IntMap_CellSizeGet(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	return intmap->GetValueCount();
}

// property int StringSize
cell_t Native_IntMap_StringSizeGet(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	return intmap->GetStringCount();
}

// property int ArraySize
cell_t Native_IntMap_ArraySizeGet(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	return intmap->GetArrayCount();
}

// public native void IterateCells(LoopCellsCallback func, any data = 0);
cell_t Native_IntMap_IterateCells(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	IPluginFunction *callbackFunction = pContext->GetFunctionById(params[1]);
	if (!callbackFunction)
	{
		return pContext->ThrowNativeError("Function id %x is invalid", params[1]);
	}

	intmap->LoopCells(callbackFunction, params);

	return 0;
}

// public native void IterateArrays(LoopArraysCallback func, any data = 0);
cell_t Native_IntMap_IterateArrays(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	IPluginFunction *callbackFunction = pContext->GetFunctionById(params[1]);
	if (!callbackFunction)
	{
		return pContext->ThrowNativeError("Function id %x is invalid", params[1]);
	}

	intmap->LoopArrays(callbackFunction, params);

	return 0;
}

// public native void IterateStrings(LoopStringsCallback func, any data = 0);
cell_t Native_IntMap_IterateStrings(IPluginContext *pContext, const cell_t *params)
{
	IntMap *intmap;
	IntMapHandler::ReadHandle(pContext, params, &intmap);

	IPluginFunction *callbackFunction = pContext->GetFunctionById(params[1]);
	if (!callbackFunction)
	{
		return pContext->ThrowNativeError("Function id %x is invalid", params[1]);
	}

	intmap->LoopStrings(callbackFunction, params);

	return 0;
}

void StringCopy(char *dest, size_t maxlength, const char *src)
{
	if (!dest || !maxlength)
	{
		return;
	}

	char *iter = dest;
	size_t count = maxlength;
	while (*src && --count)
	{
		*iter++ = *src++;
	}

	*iter = '\0';
}

SMEXT_LINK(&g_IntMap);
