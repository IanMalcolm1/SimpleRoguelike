cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project(Roguelike LANGUAGES CXX)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(Roguelike
   src/main.cpp
   src/AbilityFactory.cpp
   src/AbilityManager.cpp
   src/ActorDescriber.cpp
   src/ActorEntity.cpp
   src/ActorFactory.cpp
   src/ActorManager.cpp
   src/ActorStatBlock.cpp
   src/ActorUtils.cpp
   src/Adventure.cpp
   src/AdventureUI.cpp
   src/AIRunner.cpp
   src/Blabber.cpp
   src/Color.cpp
   src/ColorLibrary.cpp
   src/ColorPalette.cpp
   src/ConfirmerUI.cpp
   src/Damage.cpp
   src/DebugLogger.cpp
   src/Effect.cpp
   src/EffectDescriber.cpp
   src/EffectFactory.cpp
   src/EffectList.cpp
   src/EffectManager.cpp
   src/Entity.cpp
   src/ExitConfirmerUI.cpp
   src/FoV.cpp
   src/FpsCounter.cpp
   src/FpsUI.cpp
   src/Fraction.cpp
   src/Game.cpp
   src/GameLog.cpp
   src/GameOverUI.cpp
   src/GameText.cpp
   src/GameTextMaker.cpp
   src/GameWindow.cpp
   src/InputConfirmer.cpp
   src/InputManager.cpp
   src/Inventory.cpp
   src/ItemDescriber.cpp
   src/ItemEntity.cpp
   src/ItemFactory.cpp
   src/ItemManager.cpp
   src/LocalMap.cpp
   src/LookUI.cpp
   src/MapDisplay.cpp
   src/MapUI.cpp
   src/MessagesUI.cpp
   src/Pathfinding.cpp
   src/PathfindingRoute.cpp
   src/PathingSpecs.cpp
   src/PlayerManager.cpp
   src/PlayerUI.cpp
   src/Randomizer.cpp
   src/Scene.cpp
   src/SceneUI.cpp
   src/Scroller.cpp
   src/TerrainDescriber.cpp
   src/TerrainGenerator.cpp
   src/TerrainMap.cpp
   src/TerrainNameCatalogue.cpp
   src/TextRenderer.cpp
   src/TurnQueue.cpp
   src/TwoButtonPopupUI.cpp)

set_property(TARGET Roguelike PROPERTY CXX_STANDARD 20)


target_link_libraries(Roguelike ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

include_directories(Roguelike ${PROJECT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS})
