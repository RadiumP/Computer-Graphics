/*
  NOTE: This is the file you will need to begin working with
		You will need to implement the RayTrace::CalculatePixel () function

  This file defines the following:
	RayTrace Class
*/

#ifndef RAYTRACE_H
#define RAYTRACE_H

#include <stdio.h>
#include <stdlib.h>

#include "Utils.h"
#define Pi 3.1415926
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

/*
	RayTrace Class - The class containing the function you will need to implement

	This is the class with the function you need to implement
*/

class RayTrace
{
public:
	/* - Scene Variable for the Scene Definition - */
	Scene m_Scene;
	Camera m_Camera;
	SceneLight m_Light;
	int numOfLight = 1;
	// -- Constructors & Destructors --
	RayTrace (void) {}
	~RayTrace (void) {}

	// -- Main Functions --

	//-cast ray-
	Vector CastRay(int screenX, int screenY)
	{
		Vector RayDir;
		double H;//WorldCoord
		double W;
		double AR;//AspectRatio
		Vector RayPoint;
		Vector Left;
		Vector Look;

		float hOverH;
		float wOverW;
		AR = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
		m_Camera = m_Scene.GetCamera();
		H = 2.0 * m_Camera.nearClip * tan(m_Camera.GetFOV()* (Pi / 180.0) / 2.0);
		W = AR * H;

		hOverH = ((float)screenY / (float)WINDOW_HEIGHT);
		wOverW = ((float)screenX / (float)WINDOW_WIDTH);

		Look = m_Camera.GetTarget() - m_Camera.GetPosition();
		Look.Normalize();



		//Cast ray
		Left = m_Camera.GetUp().Cross(Look);
		Left.Normalize();

		RayDir = Look.operator*(m_Camera.nearClip) + m_Camera.GetUp().operator*(H*(-0.5 + hOverH)) + Left.operator* (W*(0.5 - wOverW));
		RayDir.Normalize();
		return RayDir;
	}

	//-ray tracer-
	Vector RayTracer(Vector RayOrigin, Vector RayDirection, int depth)//int depth
	{
		Vector color;
		Vector floorcolor;
		Vector wallcolor;
		SceneMaterial material;
		Vector diff;
		Vector Spec;
		float faraway = 200.0;

		//int winningObj = 0;

		m_Camera = m_Scene.GetCamera();
		float t;
		Vector normal;
		if (depth >5)return Vector(0.0, 0.0, 0.0);
		else
		{
			Vector intersectionPoint;

			for (int i = 0; i < m_Scene.GetNumObjects(); i++)
			{
				SceneObject *sceneObj = m_Scene.GetObjectA(i);
				//color = Vector(0.0, 0.0, 0.0);

				if (sceneObj->IsSphere())
				{
					SceneSphere *sphere = (SceneSphere*)sceneObj;


					t = sphere->intersection(RayOrigin, RayDirection);

					if (t > 0)
					{
						if (t < faraway)
						{

							faraway = t;
							//color = m_Scene.GetBackground().ambientLight;
							//color = Vector(0.0, 0.0, 0.0);

							intersectionPoint = RayOrigin + RayDirection.operator*(faraway);
							normal = (intersectionPoint - sphere->center).Normalize();
							material = m_Scene.GetMaterial(sphere->material);



							break;
						}
					}
					//else color = m_Scene.GetBackground().color;
				}
				
			
				
				if (sceneObj->IsTriangle())
				{
					
					if (sceneObj->name == "TriangleFloor" || sceneObj->name == "TriangleFloor2")
					{
						SceneTriangle *triangle = (SceneTriangle*)sceneObj;
						t = triangle->intersection(RayOrigin, RayDirection);

						if (t > 0.0)
						{
							if (t < faraway)
							{
								//printf("1");
								faraway = t;
								//color = Vector(0.0, 0.0, 0.0);
								intersectionPoint = RayOrigin + RayDirection.operator*(faraway);
								normal = triangle->normal[0];
								float px, py;
								if (intersectionPoint.x + intersectionPoint.z < 0)
								{
									px = (intersectionPoint.x + 30.0) / 60.0;
									py = (intersectionPoint.z + 30.0) / 60.0;

									if (fmod(8 * px, 1.f)<0.5f  && fmod(8 *py, 1.f)<0.5f)
										floorcolor = Vector(0.0, 0.0, 0.0);
									else if (fmod(8 * px, 1.f)>0.5f  && fmod(8 * py, 1.f)>0.5f)
										floorcolor = Vector(0.0, 0.0, 0.0);
									else floorcolor = Vector(1.0, 1.0, 1.0);
									break;
								}
								if (intersectionPoint.x + intersectionPoint.z>= 0)
								{
									px = 1.0 - ((-intersectionPoint.x + 30.0) / 60.0);
									py = 1.0 - ( (-intersectionPoint.z + 30.0) / 60.0);

									if (fmod(8 * px, 1.f)<0.5f  && fmod(8 * py, 1.f)<0.5f)
										floorcolor = Vector(0.0, 0.0, 0.0);
									else if (fmod(8 * px, 1.f)>0.5f  && fmod(8 * py, 1.f)>0.5f)
										floorcolor = Vector(0.0, 0.0, 0.0);
									else floorcolor = Vector(1.0, 1.0, 1.0);
									break;
								}
								if (intersectionPoint.x + intersectionPoint.y < 0)
								{
									px = (intersectionPoint.x + 30.0) / 60.0;
									py = (intersectionPoint.y + 30.0) / 60.0;

									if (fmod(8 * px, 1.f)<0.5f  && fmod(8 * py, 1.f)<0.5f)
										wallcolor = Vector(0.0, 0.0, 0.0);
									else if (fmod(8 * px, 1.f)>0.5f  && fmod(8 * py, 1.f)>0.5f)
										wallcolor = Vector(0.0, 0.0, 0.0);
									else wallcolor = Vector(1.0, 1.0, 1.0);
									break;
								}

							
								break;
							}
						}
					}
					else
					{
					
						SceneTriangle *triangle = (SceneTriangle*)sceneObj;
						
						t = triangle->intersection(RayOrigin, RayDirection);

						if (t > 0.0)
						{
							if (t < faraway)
							{
								//printf("1");
								faraway = t;
								//color = Vector(0.0, 0.0, 0.0);
								intersectionPoint = RayOrigin + RayDirection.operator*(faraway);
								normal = triangle->normal[0];
								material = m_Scene.GetMaterial(triangle->material[0]);
								break;
							}
						}
					}
				}
			}
			//-shadow & light-

			if (faraway < 200.0)
			{
				int shadowFlag = 0;
				Vector Diff, Spec;
				Vector Amb = Vector(0.0, 0.0, 0.0);
				float win = 200.0;
				Vector Reflection;
				for (int n = 0; n < numOfLight; n++)
				{
					Vector L;
					Vector N;
					Vector V;

					Vector R;

					

					V = (m_Camera.GetTarget() - m_Camera.GetPosition()).Normalize();
					L = (m_Scene.GetLight(n).position - intersectionPoint).Normalize();
					for (int i = 2; i < m_Scene.GetNumObjects(); i++)//get
					{


						SceneObject *sceneObj = m_Scene.GetObjectA(i);

						SceneSphere *sphere;

						SceneTriangle *triangle;

						float e = 0.01;

						if (sceneObj->IsSphere())
						{
							
							float t1;
							sphere = (SceneSphere*)sceneObj;

							//V = (m_Camera.GetTarget() - m_Camera.GetPosition()).Normalize();
							//L = (m_Scene.GetLight(n).position - intersectionPoint).Normalize();
							//N = (intersectionPoint - sphere->center).Normalize();
							//Vector hereNormal = (intersectionPoint - sphere->center).Normalize();

							N = normal;

							//Reflection = (V - hereNormal.operator*(2 * (V.Dot(hereNormal)))).Normalize();
							Reflection = R;
							R = (V - N.operator*(2 * (V.Dot(N)))).Normalize();
							Diff =  material.diffuse.operator*(max(0.0, L.Dot(N)));
							Spec =  material.specular.operator*(powf(max(0.0, V.Dot(R)), material.shininess));
							Amb = material.ambient;
							t1 = sphere->intersection(intersectionPoint + L.operator*(e), L);
							if (t1 > 0.0)//shadow
							{
								if (t1 < win)
								{
									win = t1;
									shadowFlag = 1;
									//printf("Y");
									//color = Vector(0.0, 0.0, 0.0);
									break;
								}
							}


						}
						
						/*
						if (sceneObj->IsTriangle())
						{
							float t2;
							triangle = (SceneTriangle*)sceneObj;

							//V = (m_Camera.GetTarget() - m_Camera.GetPosition()).Normalize();
							//L = (m_Scene.GetLight(n).position - intersectionPoint).Normalize();
							Vector hereNormal = triangle->normal[0];

							N = normal;

							//Reflection = (V - hereNormal.operator*(2 * (V.Dot(hereNormal)))).Normalize();
							Reflection = R;
							R = (V - N.operator*(2 * (V.Dot(N)))).Normalize();
							Diff = material.diffuse.operator*(max(0.0, L.Dot(N)));
							Spec = material.specular.operator*(powf(max(0.0, V.Dot(R)), material.shininess));
							Amb = material.ambient;
							t2 = triangle->intersection(intersectionPoint + L.operator*(e), L);

							if (t2 > 0.0)//shadow
							{
								if (t2 < win)
								{

									shadowFlag = 1;
									//printf("Y");
									//color = Vector(0.0, 0.0, 0.0);
									break;
								}
							}
							//break;
						}
						*/
					}

					//else 
				
				}
				if (shadowFlag == 0)
				{
					if ( intersectionPoint.y < -4.6)
					{
						color = floorcolor + RayTracer(intersectionPoint + Reflection.operator*(0.01), Reflection, depth + 1).operator*(0.2);
					}
					
					else
					{

						color = Amb + Diff + Spec + RayTracer(intersectionPoint + Reflection.operator*(0.01), Reflection, depth + 1).operator*(0.2);
					}
				}
				else if (shadowFlag == 1)
				{
					color = Amb;

				}
				//else if(intersectionPoint.x)color = floorcolor;
			}
			
			else color = Vector(0.0, 0.0, 0.0);

			return color;
			}
		}
	

	// - CalculatePixel - Returns the Computed Pixel for that screen coordinate
	Vector CalculatePixel (int screenX, int screenY)
	{
		/*
			-- How to Implement a Ray Tracer --

			This computed pixel will take into account the camera and the scene
				and return a Vector of <Red, Green, Blue>, each component ranging from 0.0 to 1.0

			In order to start working on computing the color of this pixel,
				you will need to cast a ray from your current camera position
				to the image-plane at the given screenX and screenY
				coordinates and figure out how/where this ray intersects with 
				the objects in the scene descriptor.
				The Scene Class exposes all of the scene's variables for you 
				through its functions such as m_Scene.GetBackground (), m_Scene.GetNumLights (), 
				etc. so you will need to use those to learn about the World.

			To determine if your ray intersects with an object in the scene, 
				you must calculate where your objects are in 3D-space [using 
				the object's scale, rotation, and position is extra credit]
				and mathematically solving for an intersection point corresponding to that object.
			
			For example, for each of the spheres in the scene, you can use 
				the equation of a sphere/ellipsoid to determine whether or not 
				your ray from the camera's position to the screen-pixel intersects 
				with the object, then from the incident angle to the normal at 
				the contact, you can determine the reflected ray, and recursively 
				repeat this process capped by a number of iterations (e.g. 10).

			Using the lighting equation to calculate the color at every 
				intersection point and adding its fractional amount (determined by the material)
				will get you a final color that returns to the eye at this point.
		*/

		if ((screenX < 0 || screenX > WINDOW_WIDTH - 1) ||
			(screenY < 0 || screenY > WINDOW_HEIGHT - 1))
		{
			// Off the screen, return black
			return Vector (0.0f, 0.0f, 0.0f);
		}

		Vector RayDirection  = CastRay(screenX, screenY);
		Vector color = RayTracer(m_Camera.GetPosition(),RayDirection,0);


		// Until this function is implemented, return white
		return color;
		//return Vector (1.0f, 1.0f, 1.0f);
	}
};

#endif // RAYTRACE_H
