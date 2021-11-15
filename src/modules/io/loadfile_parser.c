#include <stdio.h>

#include "../../models/location.h"
#include "../../models/location_list.h"
#include "../../models/point.h"
#include "../../models/boolean_matrix.h"
#include "../../models/item_queue.h"
#include "../../models/item.h"
#include "../../models/game_map.h"
#include "../../models/macros.h"
#include "../core/globals.h"
#include "./machines/wordmachine.h"
#include "word_utils.h"

void parseLoad(char *path)
{
    // * Read config file path
    readFile(path);
    // * Set map size
    int mapLength, mapWidth;
    readWord();
    mapLength = parseInt(stringify(currentWord));
    readNextWord();
    mapWidth = parseInt(stringify(currentWord));

    // * Parse HQ location
    Location hq;
    Point hqCoord;
    int hq_abs, hq_ord;
    readNextWord();
    hq_abs = parseInt(stringify(currentWord));
    readNextWord();
    hq_ord = parseInt(stringify(currentWord));
    hqCoord = newPoint(hq_abs, hq_ord);
    hq = newLocation(0, '8', hqCoord);

    // * Locations list
    int locationCount;
    readNextWord();
    locationCount = parseInt(stringify(currentWord)) + 1;

    LocationList lList = newLocationList(locationCount);
    insertLast(&lList, hq);
    Location loc;
    Point coord;
    int location_abs, location_ord;
    char symbol;

    for (int i = 1; i < locationCount; i++)
    {
        readNextWord();
        symbol = stringify(currentWord)[0];
        readNextWord();
        location_abs = parseInt(stringify(currentWord));
        readNextWord();
        location_ord = parseInt(stringify(currentWord));
        coord = newPoint(location_abs, location_ord);
        loc = newLocation(i, symbol, coord);

        insertLast(&lList, loc);
    }

    // * Adjacency matrix
    BooleanMatrix adjMatrix;
    adjMatrix = newBooleanMatrix(locationCount, locationCount);

    for (int i = 0; i < locationCount; i++)
    {
        for (int j = 0; j < locationCount; j++)
        {
            readNextWord();
            elem(adjMatrix, i, j) = parseInt(stringify(currentWord));
        }
    }

    int itemCount;
    int orderTime, perishTime, itemType, perishTimeReference;
    char pickUpLocSymbol, dropOffLocSymbol, type;
    Location pickUpLocation, dropOffLocation;
    Item item;

    //todoList
    readNextWord();
    ItemList todoList = newItemList();
    itemCount = parseInt(stringify(currentWord));
    printf("%d\n", itemCount);

    for (int i = 0; i < itemCount; i++)
    {
        readNextWord();
        orderTime = parseInt(stringify(currentWord));
        readNextWord();
        pickUpLocSymbol = stringify(currentWord)[0];
        pickUpLocation = _getLocationBySymbol(lList, pickUpLocSymbol);
        readNextWord();
        dropOffLocSymbol = stringify(currentWord)[0];
        dropOffLocation = _getLocationBySymbol(lList, dropOffLocSymbol);
        readNextWord();
        type = stringify(currentWord)[0];
        switch (type)
        {
        case 'N':
            itemType = NORMAL;
            break;
        case 'H':
            itemType = HEAVY;
            break;
        case 'P':
            itemType = PERISHABLE;
            break;
        case 'V':
            itemType = VIP;
            break;
        }

        if (itemType == PERISHABLE)
        {
            readNextWord();
            perishTime = parseInt(stringify(currentWord));
            readNextWord();
            perishTimeReference = parseInt(stringify(currentWord));
        }
        else
        {
            perishTime = UNTIMED;
            perishTimeReference = UNTIMED;
        }
        item = newItem(orderTime, pickUpLocation, dropOffLocation, itemType, perishTime, perishTimeReference);
        insertItemLast(&todoList, item);
    }

    //inprogress
    readNextWord();
    ItemList inProgressList = newItemList();
    itemCount = parseInt(stringify(currentWord));
    printf("%d\n", itemCount);

    for (int i = 0; i < itemCount; i++)
    {
        readNextWord();
        orderTime = parseInt(stringify(currentWord));
        readNextWord();
        pickUpLocSymbol = stringify(currentWord)[0];
        pickUpLocation = _getLocationBySymbol(lList, pickUpLocSymbol);
        readNextWord();
        dropOffLocSymbol = stringify(currentWord)[0];
        dropOffLocation = _getLocationBySymbol(lList, dropOffLocSymbol);
        readNextWord();
        type = stringify(currentWord)[0];
        switch (type)
        {
        case 'N':
            itemType = NORMAL;
            break;
        case 'H':
            itemType = HEAVY;
            break;
        case 'P':
            itemType = PERISHABLE;
            break;
        case 'V':
            itemType = VIP;
            break;
        }

        if (itemType == PERISHABLE)
        {
            readNextWord();
            perishTime = parseInt(stringify(currentWord));
            readNextWord();
            perishTimeReference = parseInt(stringify(currentWord));
        }
        else
        {
            perishTime = UNTIMED;
            perishTimeReference = UNTIMED;
        }
        item = newItem(orderTime, pickUpLocation, dropOffLocation, itemType, perishTime, perishTimeReference);
        insertItemLast(&inProgressList, item);
    }

    //bag
    readNextWord();
    itemCount = parseInt(stringify(currentWord));
    printf("%d\n", itemCount);
    ItemStack bag = newItemStack(itemCount);
    ItemStack tempBag;
    for (int i = 0; i < itemCount; i++)
    {
        readNextWord();
        orderTime = parseInt(stringify(currentWord));
        readNextWord();
        pickUpLocSymbol = stringify(currentWord)[0];
        pickUpLocation = _getLocationBySymbol(lList, pickUpLocSymbol);
        readNextWord();
        dropOffLocSymbol = stringify(currentWord)[0];
        dropOffLocation = _getLocationBySymbol(lList, dropOffLocSymbol);
        readNextWord();
        type = stringify(currentWord)[0];
        switch (type)
        {
        case 'N':
            itemType = NORMAL;
            break;
        case 'H':
            itemType = HEAVY;
            break;
        case 'P':
            itemType = PERISHABLE;
            break;
        case 'V':
            itemType = VIP;
            break;
        }

        if (itemType == PERISHABLE)
        {
            readNextWord();
            perishTime = parseInt(stringify(currentWord));
            readNextWord();
            perishTimeReference = parseInt(stringify(currentWord));
        }
        else
        {
            perishTime = UNTIMED;
            perishTimeReference = UNTIMED;
        }
        item = newItem(orderTime, pickUpLocation, dropOffLocation, itemType, perishTime, perishTimeReference);
        push(&bag, item);
    }

    Item tempItem;
    tempBag = newItemStack(capacity(bag));
    while (!isStackEmpty(bag))
    {
        pop(&bag, &tempItem);
        push(&tempBag, tempItem);
    }

    //order
    readNextWord();
    itemCount = parseInt(stringify(currentWord));

    ItemQueue order = newItemQueue();

    for (int i = 0; i < itemCount; i++)
    {
        readNextWord();
        orderTime = parseInt(stringify(currentWord));
        readNextWord();
        pickUpLocSymbol = stringify(currentWord)[0];
        pickUpLocation = _getLocationBySymbol(lList, pickUpLocSymbol);
        readNextWord();
        dropOffLocSymbol = stringify(currentWord)[0];
        dropOffLocation = _getLocationBySymbol(lList, dropOffLocSymbol);
        readNextWord();
        type = stringify(currentWord)[0];
        switch (type)
        {
        case 'N':
            itemType = NORMAL;
            break;
        case 'H':
            itemType = HEAVY;
            break;
        case 'P':
            itemType = PERISHABLE;
            break;
        case 'V':
            itemType = VIP;
            break;
        }

        if (itemType == PERISHABLE)
        {
            readNextWord();
            perishTime = parseInt(stringify(currentWord));
            readNextWord();
            perishTimeReference = parseInt(stringify(currentWord));
        }
        else
        {
            perishTime = UNTIMED;
            perishTimeReference = UNTIMED;
        }
        item = newItem(orderTime, pickUpLocation, dropOffLocation, itemType, perishTime, perishTimeReference);
        enqueue(&order, item);
    }

    GameMap g = newGameMap(mapLength, mapWidth, adjMatrix, lList);
    gameState = newState(g, todoList, inProgressList, tempBag, order);
    readNextWord();
    gameState.time = parseInt(stringify(currentWord));
    readNextWord();
    gameState.cash = parseInt(stringify(currentWord));
    int gadgetID;

    GadgetList tempGList = newGadgetList();
    for (int i = 0; i < 5; i++)
    {
        readNextWord();
        printf("%s\n", stringify(currentWord));
        gadgetID = parseInt(stringify(currentWord));
        printf("Gadget id : %d\n", gadgetID);

        switch (gadgetID)
        {
        case 0:
            setGadget(&tempGList, i, KAIN_PEMBUNGKUS_WAKTU);
        case 1:
            setGadget(&tempGList, i, SENTER_PEMBESAR);
        case 2:
            setGadget(&tempGList, i, PINTU_KEMANA_SAJA);
        case 3:
            setGadget(&tempGList, i, MESIN_WAKTU);
        case 4:
            setGadget(&tempGList, i, SENTER_PENGECIL);
        case -1:
            setGadget(&tempGList, i, NULL_GADGET);
        default:
            break;
        }
    }
    displayGadget(tempGList);
    readNextWord();
    char *playerLocSymbol = stringify(currentWord);
    readNextWord();
    int playerAbs = parseInt(stringify(currentWord));
    readNextWord();
    int playerOrd = parseInt(stringify(currentWord));
    setPlayerLocation(&gameState.gameMap, playerAbs, playerOrd);
    setAsPlayerPlace(&gameState.gameMap._locationMatrix.contents[playerAbs][playerOrd]);
    gameState.currentLocation = gameState.gameMap._locationMatrix.contents[playerAbs][playerOrd];
    readNextWord();

    int sbFlag = parseInt(stringify(currentWord));
    if (sbFlag)
    {
        gameState.abs.SpeedBoost = true;
    }
    readNextWord();
    int sbCount = parseInt(stringify(currentWord));
    gameState.abs.SpeedBoostCount = sbCount;
    readNextWord();
    int sbStack = parseInt(stringify(currentWord));
    gameState.abs.SpeedBoostStack = sbStack;

    readNextWord();
    int spOn = parseInt(stringify(currentWord));
    if (spOn)
    {
        gameState.abs.IsSenterPengecilOn = true;
    }

    readNextWord();
    int pintu = parseInt(stringify(currentWord));
    if (pintu)
    {
        gameState.abs.PintuKemanaSaja = true;
    }

    readNextWord();
    int heavyFlag = parseInt(stringify(currentWord));
    if (heavyFlag)
    {
        gameState.abs.IsHeavyItemOn = true;
    }

    readNextWord();
    int heavyStack = parseInt(stringify(currentWord));
    if (heavyFlag)
    {
        gameState.abs.HeavyItemStack = heavyStack;
    }

    readNextWord();
    int vipFlag = parseInt(stringify(currentWord));
    if (vipFlag)
    {
        gameState.abs.IsVIPItemOn = true;
    }

    readNextWord();
    int vipStack = parseInt(stringify(currentWord));
    if (vipFlag)
    {
        gameState.abs.VIPItemStack = vipStack;
    }

    readNextWord();
    int todoVIP = parseInt(stringify(currentWord));
    gameState.abs.TodoVIP = todoVIP;

    readNextWord();
    int returnFlag = parseInt(stringify(currentWord));
    if (returnFlag)
    {
        gameState.abs.IsReturnOn = true;
    }

    readNextWord();
    int returnStack = parseInt(stringify(currentWord));
    if (returnFlag)
    {
        gameState.abs.ReturnStack = returnStack;
    }

    printf("File save berhasil diload!\n");
}