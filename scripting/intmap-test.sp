#include <sourcemod>
#include <intmaps>

public void OnPluginStart()
{
	// public native IntMap();
	IntMap map = new IntMap();

	// public native void SetValue(const int key, any value);
	// public native any  GetValue(const int key);
	{
		map.SetValue(5, 10);

		PrintToServer("%i", map.GetValue(5));
		PrintToConsoleAll("%i", map.GetValue(5));

		// 10
	}

	// public native void SetArray(const int key, any[] value, int maxlength);
	// public native void GetArray(const int key, any[] value, int maxlength);
	{
		float input[2] = {15.0, -15.0};
		map.SetArray(5, input, 2);
		
		float arr[2];
		map.GetArray(5, arr, 2);

		PrintToServer("%f %f", arr[0], arr[1]);
		PrintToConsoleAll("%f %f", arr[0], arr[1]);

		// 15.000000 -15.000000
	}

	// public native any  GetArrayCell(const int key, int index);
	{
		PrintToServer("{%f %f}", map.GetArrayCell(5, 0), map.GetArrayCell(5, 1));
		PrintToConsoleAll("{%f %f}", map.GetArrayCell(5, 0), map.GetArrayCell(5, 1));

		// {15.000000 -15.000000}
	}

	// public native void SetString(const int key, const char[] value);
	// public native void GetString(const int key, char[] value, int maxlength);
	{	
		char str[32] = "this is a test";
		map.SetString(1, str);

		char result[32];
		map.GetString(1, result, 32);

		PrintToServer(result);
		PrintToConsoleAll(result);

		// this is a test
	}

	// public native void RemoveCell(const int key);
	// public native void RemoveString(const int key);
	// public native void RemoveArray(const int key);
	{
		map.RemoveCell(5);
		map.RemoveString(5);
		map.RemoveArray(5);

		char buffer[32];
		map.GetString(5, buffer, 32);

		PrintToServer("%i '%s' %f", map.GetValue(5), buffer, map.GetArrayCell(5, 0));
		PrintToConsoleAll("%i '%s' %f", map.GetValue(5), buffer, map.GetArrayCell(5, 0));
	}

	// property int CellSize
	// property int StringSize
	// property int ArraySize
	{
		PrintToServer("size %i %i %i", map.CellSize, map.StringSize, map.ArraySize);
		PrintToConsoleAll("size %i %i %i", map.CellSize, map.StringSize, map.ArraySize);
	}

	// public native void RemoveAll(const int key);
	{
		map.SetValue(11, 10);
		map.SetArray(11, {10}, 1);
		map.SetValue(11, "10");

		PrintToServer("bf %i %i %i", map.CellSize, map.StringSize, map.ArraySize);
		PrintToConsoleAll("bf %i %i %i", map.CellSize, map.StringSize, map.ArraySize);

		map.RemoveAll(11);

		PrintToServer("af %i %i %i", map.CellSize, map.StringSize, map.ArraySize);
		PrintToConsoleAll("af %i %i %i", map.CellSize, map.StringSize, map.ArraySize);
	}

	// public native void ClearCells();
	// public native void ClearArrays();
	// public native void ClearStrings();
	// public native void ClearAll();
	{
		map.SetValue(10, 10);
		map.SetArray(10, {10}, 1);
		map.SetValue(10, "10");

		PrintToServer("bf %i %i %i", map.CellSize, map.StringSize, map.ArraySize);
		PrintToConsoleAll("bf %i %i %i", map.CellSize, map.StringSize, map.ArraySize);

		map.ClearCells();
		map.ClearArrays();
		map.ClearStrings();

		PrintToServer("mid %i %i %i", map.CellSize, map.StringSize, map.ArraySize);
		PrintToConsoleAll("mid %i %i %i", map.CellSize, map.StringSize, map.ArraySize);

		map.SetValue(10, 10);
		map.SetArray(10, {10}, 1);
		map.SetValue(10, "10");

		map.ClearAll();

		PrintToServer("af %i %i %i", map.CellSize, map.StringSize, map.ArraySize);
		PrintToConsoleAll("af %i %i %i", map.CellSize, map.StringSize, map.ArraySize);
	}

	// public native bool HasValue(const int key);
	// public native bool HasArray(const int key);
	// public native bool HasString(const int key);
	{
		PrintToServer("has %i %i %i", map.HasValue(20), map.HasArray(20), map.HasString(20));
		PrintToConsoleAll("has %i %i %i", map.HasValue(20), map.HasArray(20), map.HasString(20));


		map.SetValue(20, 10);
		map.SetArray(20, {10}, 1);
		map.SetValue(20, "10");
	}

	// public native void IterateCells(LoopCellsCallback func, any data = 0);
	{
		for(int i = 0; i < 20; ++i)
		{
			map.SetValue(i, i);
		}
		map.IterateCells(ForEachCell, 5);
	}

	// public native void IterateArrays(LoopArraysCallback func, any data = 0);
	{
		// create 20 arrays with the size and value of the current iteration
		for(int i = 0; i < 20; ++i)
		{
			int[] arr = new int[i+1];

			for(int j = 0; j <= i; ++j)
			{
				arr[j] = j;
			}

			map.SetArray(i, arr, i+1);
		}

		map.IterateArrays(ForEachArray, 10);
	}

	// public native void IterateStrings(LoopStringsCallback func, any data = 0);
	{
		for(int i = 0; i < 10; ++i)
		{
			char[] arr = new char[i+1];

			for(int j = 0; j < i; ++j)
			{
				arr[j] = j+48;
			}

			map.SetString(i, arr, i);
		}

		map.IterateStrings(ForEachString, 100);
	}

	delete map;
}

public Action ForEachCell(IntMap map, int key, any value, any data)
{
	PrintToServer("<%i %i>(%f)", key, value, data);
	PrintToConsoleAll("<%i %i>(%i)", key, value, data);

	if(key == 15)
	{
		return Plugin_Handled;
	}

	return Plugin_Continue;
}

public Action ForEachArrayAction(IntMap map, int key, const any[] values, int size, any data);
{
	PrintToServer("<%i %s>[%i](%i)", key, value, data);
	PrintToConsoleAll("<%i %s>[%i](%i)", key, value, data);

	if(key == 15)
	{
		return Plugin_Handled;
	}

	return Plugin_Continue;
}

public Action ForEachCell(IntMap map, int key, any value, any data);
{
	PrintToServer("<%i %i>(%f)", key, value, data);
	PrintToConsoleAll("<%i %i>(%i)", key, value, data);

	if(key == 15)
	{
		return Plugin_Handled;
	}

	return Plugin_Continue;
}