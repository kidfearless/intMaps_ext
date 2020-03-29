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
	
	delete map;
}