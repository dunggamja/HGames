#pragma once



#include <memory>
#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <list>
#include <algorithm>
#include <random>
#include <stdint.h>
//#include <optional>  C++17
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <ppl.h>
#include <cmath>
#include <limits>

//========================================================================
// Math Lib - GMTL (Graphics Math Template Library)
// https://stackoverflow.com/questions/1380371/what-are-the-most-widely-used-c-vector-matrix-math-linear-algebra-libraries-a
// http://ggt.sourceforge.net/gmtlReferenceGuide-0.6.1-html/index.html
//========================================================================
#include <gmtl/gmtl.h>


//========================================================================
// BASE
//========================================================================
#include "Base/Define.h"
#include "Base/Singleton.h"
#include "Base/MathHelper.h"
#include "Base/PreCompiler.h"
#include "Base/BaseObject.h"



//========================================================================
// PHYSICS
//========================================================================
#include "Physics/Collide/Collide.h"
#include "Physics/Rigidbody/Rigidbody.h"
#include "Physics/Contact/Contact.h"
#include "Physics/Contact/ContactResolver.h"
#include "Physics/Space/Grid.h"
#include "Physics/PhysicsManager.h"

