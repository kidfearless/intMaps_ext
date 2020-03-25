#include <sourcemod>
#include <intmaps>

public void OnPluginStart()
{
	IntMap map = new IntMap();

	map.SetValue(5, 10);

	PrintToServer("%i", map.GetValue(5));
	PrintToConsoleAll("%i", map.GetValue(5));

	float input[2] = {15.0, -15.0};
	map.SetArray(5, input, 2);
	
	float arr[2];
	map.GetArray(5, arr, 2);

	PrintToServer("%f %f", arr[0], arr[1]);
	PrintToConsoleAll("%f %f", arr[0], arr[1]);

	PrintToServer("{%f %f}", map.GetArrayCell(5, 0), map.GetArrayCell(5, 1));
	PrintToConsoleAll("{%f %f}", map.GetArrayCell(5, 0), map.GetArrayCell(5, 1));



	delete map;
}