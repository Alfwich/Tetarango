#pragma once

// MT
#include "engine/Application.h"
#include "ui/renderable/primitive/particle/ParticleSystem.h"
#include "ui/renderable/element/Rectangle.h"
#include "ui/renderable/element/Text.h"
#include "ui/scene/Scene.h"
#include "ui/scene/SceneContainer.h"
#include "ui/renderable/element/tilemap/TileMap.h"
#include "ui/renderable/element/Text.h"
#include "ui/renderable/element/Backdrop.h"
#include "ui/renderable/primitive/trace/Trace.h"

// Scenes
#include "scene/Scenes.h"
#include "scene/test/TestScene.h"
#include "scene/test/TestSpaceScene.h"
#include "scene/menu/SceneSplash.h"
#include "scene/menu/SceneMainMenu.h"
#include "scene/menu/SceneOptionsMenu.h"
#include "scene/menu/SceneSavedGamesMenu.h"
#include "scene/game/SceneMainGame.h"
#include "scene/game/tetris/SceneTetris.h"

// Props
#include "prop/BaseProp.h"
#include "prop/block/Block.h"

// Actor
#include "actor/board/Board.h"

// Particles
#include "prop/particles/projectile/bullet/ParticleBulletTrailFactory.h"
#include "prop/particles/TestParticleFactory.h"
#include "prop/particles/firing/ParticleFiringFactory.h"
#include "prop/particles/hit/flesh/ParticleHitFleshFactory.h"
#include "prop/particles/hit/hard/ParticleHitHardFactory.h"
#include "prop/particles/hit/soft/ParticleHitSoftFactory.h"
#include "prop/particles/space/ParticleSpaceBackgroundFactory.h"
#include "prop/particles/block/BlockParticleFactory.h"

// General
#include "gui/camera/GameCamera.h"

// GUI
#include "gui/button/ButtonBasic.h"
#include "gui/button/CheckBoxBasic.h"
#include "gui/menu/GameMainMenu.h"
#include "gui/transition/TransitionFade.h"
