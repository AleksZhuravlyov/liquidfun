/*
* Copyright (c) 2013 Google, Inc.
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/
#ifndef FOLDEDGEOLOGY_H
#define FOLDEDGEOLOGY_H

class FoldedGeology : public Test {
public:

  FoldedGeology() {
    // Setup particle parameters.
    TestMain::SetParticleParameters(k_paramDef, k_paramDefCount);

    {
      b2BodyDef bd;
      b2Body *ground = m_world->CreateBody(&bd);

      b2Vec2 gravity(0.0f, 10.0f);
      m_world->SetGravity(gravity);

      // Shape of main layout
      b2ChainShape shape;
      const b2Vec2 vertices[4] = {
          b2Vec2(-2, 0),
          b2Vec2(2, 0),
          b2Vec2(2, 4),
          b2Vec2(-2, 4)};
      shape.CreateLoop(vertices, 4);
      ground->CreateFixture(&shape, 0.0f);

    }

    // Radius of particles
    m_particleSystem->SetRadius(0.02f);
    {
      b2PolygonShape shape;
      // Size and position of particles
      shape.SetAsBox(2, 0.4f, b2Vec2(0, 0.6f), 0);
      b2ParticleGroupDef pd;
      pd.flags = TestMain::GetParticleParameterValue();
      pd.shape = &shape;
      b2ParticleGroup *const group = m_particleSystem->CreateParticleGroup(
          pd);
      if (pd.flags & b2_colorMixingParticle) {
        ColorParticleGroup(group, 0);
      }
    }

    {
      // bottom border coords
      b2BodyDef bd;
      b2Body *body = m_world->CreateBody(&bd);
      b2EdgeShape shape;
      shape.Set(b2Vec2(-2, 1.f), b2Vec2(0.6f, 1.f));
      body->CreateFixture(&shape, 0.1f);
    }

    {
      // right border coords
      b2BodyDef bd;
      b2Body *body = m_world->CreateBody(&bd);
      b2EdgeShape shape;
      shape.Set(b2Vec2(0.7f, 1.f), b2Vec2(2, 1.f));
      body->CreateFixture(&shape, 0.1f);
    }

    {
      // source shape left coords
      b2BodyDef bd;
      b2Body *body = m_world->CreateBody(&bd);
      b2EdgeShape shape;
      shape.Set(b2Vec2(0.6f, 1.f), b2Vec2(0.6f, 1.4f));
      body->CreateFixture(&shape, 0.1f);
    }

    {
      // source shape right coords
      b2BodyDef bd;
      b2Body *body = m_world->CreateBody(&bd);
      b2EdgeShape shape;
      shape.Set(b2Vec2(0.7f, 1.f), b2Vec2(0.7f, 1.4f));
      body->CreateFixture(&shape, 0.1f);
    }

    // handmade thing (right part of bed)
    {
      b2BodyDef bd;
      bedRight = m_world->CreateBody(&bd);
      b2EdgeShape shape;


      float32 xs[33] = {-1.0, -1.1, -1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4,
                        -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6,
                        0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7,
                        1.8, 1.9, 2.0};

      float32 ys[33] = {4.00, 2.70, 2.70, 2.78, 2.94, 3.06, 3.22, 3.34, 3.43,
                        3.49, 3.50, 3.47, 3.41, 3.31, 3.18, 3.02, 2.83, 2.64,
                        2.44, 2.24, 2.06, 1.89, 1.74, 1.63, 1.55, 1.51, 1.50,
                        1.54, 1.62, 1.73, 1.87, 2.04, 2.2};

      for (int32 i = 0; i < 32; ++i) {
        shape.Set(b2Vec2(xs[i], ys[i]), b2Vec2(xs[i + 1], ys[i + 1]));
        bedRight->CreateFixture(&shape, 0.1f);
      }

    }

    // handmade thing (left part of bed)
    {
      b2BodyDef bd;
      bedLeft = m_world->CreateBody(&bd);
      b2EdgeShape shape;


      float32 xs[11] = {-2.1, -2.0, -1.9, -1.8, -1.7, -1.6, -1.5, -1.4, -1.3,
                        -1.2, -1.1};

      float32 ys[11] = {2.70, 2.73, 2.85, 3.02, 3.19, 3.29, 3.29, 3.19, 3.02,
                        2.85, 2.7};

      for (int32 i = 0; i < 10; ++i) {
        shape.Set(b2Vec2(xs[i], ys[i]), b2Vec2(xs[i + 1], ys[i + 1]));
        bedLeft->CreateFixture(&shape, 0.1f);
      }

    }

  }


  // Run a simulation step.
  void Step(Settings *settings) {
    const float32 dt = 1.0f / settings->hz;
    Test::Step(settings);

    static const char *k_keys[] = {"Keys: (f) fault"};

    for (uint32 i = 0; i < B2_ARRAY_SIZE(k_keys); ++i) {
      m_debugDraw.DrawString(5, m_textLine, k_keys[i]);
      m_textLine += DRAW_STRING_NEW_LINE;
    }

  }

  // Simulate fault moving left part of bed.
  void Keyboard(unsigned char key) {
    uint32 parameter = 0;
    switch (key) {
      case 'f':
        bedLeft->SetTransform(
            bedLeft->GetPositionX() + 0.01 / 13.,
            bedLeft->GetPositionY() + 0.01,
            0);
        break;
      default:
        // Nothing.
        return;
    }
  }


  float32 GetDefaultViewZoom() const {
    return 0.1f;
  }

  static Test *Create() {
    return new FoldedGeology;
  }

  static const ParticleParameter::Value k_paramValues[];
  static const ParticleParameter::Definition k_paramDef[];
  static const uint32 k_paramDefCount;

private:
  b2Body *bedRight;
  b2Body *bedLeft;

};

const ParticleParameter::Value FoldedGeology::k_paramValues[] =
    {
        {b2_tensileParticle | b2_viscousParticle,
            ParticleParameter::k_DefaultOptions, "tensile + viscous"},
    };
const ParticleParameter::Definition FoldedGeology::k_paramDef[] =
    {
        {
            FoldedGeology::k_paramValues,
            B2_ARRAY_SIZE(FoldedGeology::k_paramValues)
        },
        {
            ParticleParameter::k_particleTypesPtr,
            ParticleParameter::k_particleTypesCount
        },
    };
const uint32 FoldedGeology::k_paramDefCount =
    B2_ARRAY_SIZE(FoldedGeology::k_paramDef);


#endif
