#include "main.h" // TAG_REMOVE

int main()
{	
	// game loop
	while (1) {
		high_resolution_clock::time_point turnStartTime = high_resolution_clock::now();
		GC.StartNextTurn();
		int myShipCount; // the number of remaining ships
		cin >> myShipCount; cin.ignore();
		int entityCount; // the number of entities (e.g. ships, mines or cannonballs)
		cin >> entityCount; cin.ignore();
		// Input processing loop
		for (int i = 0; i < entityCount; i++)
		{
			int entityId;
			string entityType;
			int x;
			int y;
			int arg1;
			int arg2;
			int arg3;
			int arg4;
			cin >> entityId >> entityType >> x >> y >> arg1 >> arg2 >> arg3 >> arg4; cin.ignore();

			if (entityType == "SHIP" && arg4 == 1)
			{
				GC.UpdatePlayerShip(entityId, x, y, arg2, arg1, arg3);
			}
			else if (entityType == "SHIP" && arg4 == 0)
			{
				GC.UpdateEnemyShip(entityId, x, y, arg2, arg1, arg3);
			}
			else if (entityType == "BARREL")
			{
				GC.AddBarrel(entityId, x, y, arg1);
			}
			else if (entityType == "CANNONBALL")
			{
				GC.AddCannonball(entityId, x, y, arg1, arg2);
			}
			else if (entityType == "MINE")
			{
				GC.AddMine(entityId, x, y);
			}
			else
			{
				cerr << "ERROR: Unhandled entity type: " << entityType << endl;
			}
		}

		GC.InputComplete();

		high_resolution_clock::time_point inputReadFinishTime = high_resolution_clock::now();

		GC.OutputMoves();


		high_resolution_clock::time_point processingFinishTime = high_resolution_clock::now();

		cerr << "Turn time: " << chrono::duration_cast<chrono::milliseconds>(processingFinishTime - turnStartTime).count()
			<< " ms"
			<< " Input Read time: " << chrono::duration_cast<chrono::milliseconds>(inputReadFinishTime - turnStartTime).count()
			<< " ms" << endl;
	}
}