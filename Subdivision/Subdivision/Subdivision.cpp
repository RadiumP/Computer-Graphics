#include <fstream>
#include <vector>

#include "OBJFileReader.h"
#include "Solid.h"
#include "iterators.h"
#include "SolidDelegate.h"
#include <unordered_map>

using namespace MeshLib;

void main(int argc, char *argv[])
{
	// Read in the obj file
	Solid mesh;
	Solid newMesh;
	OBJFileReader of;
	std::ifstream in(argv[1]);
	of.readToSolid(&mesh, in);

	/******************* Put you subdivision processing here *********************/

	
	SolidDelegate sodel;
	SolidVertexIterator veriter(&mesh);
	SolidFaceIterator faceiter(&mesh);
	SolidEdgeIterator edgeiter(&mesh);
	std::unordered_map<Edge*, Vertex*> edgemap;
	
	for (; !veriter.end(); ++veriter)
	{
		Vertex *vertmp = *veriter;
		sodel.createVertex(&newMesh, vertmp->id());
		
	}

	
	for (; !faceiter.end(); ++faceiter)
	{

		Vertex **v = new Vertex*[9];
		Vertex **newv = new Vertex*[6];
		
		Face *face = *faceiter;
		Edge *edge;
		HalfEdge *he = face->halfedge();
		HalfEdge *temp;
		
	
		
		v[0] = he->source();
		v[1] = he->target();
		temp = he->he_next();
		v[2] = temp->target();
		

		//even old points new position
		for (int i = 0; i < 3; i++)
		{
			Point newPnt;
			edge = he->edge();
			int num = newMesh.numVertices();

		//check boundary
			if (mesh.isBoundary(v[i]))
			{
				
				Vertex *ccv,*clv;
				HalfEdge *cche, *clhe;
				Point ccp, clp, pp;
				
				pp = v[i]->point();
			
				
				cche = v[i]->most_ccw_in_halfedge();
				ccv = cche->source();
				
				
				ccp = ccv->point();
				clp = v[(i+1)%3]->point();
				
				newPnt[0] = (ccp[0] + clp[0] + 6.0*pp[0]) / 8.0;
				newPnt[1] = (ccp[1] + clp[1] + 6.0*pp[1]) / 8.0;
				newPnt[2] = (ccp[2] + clp[2] + 6.0*pp[2]) / 8.0;
				

			
			}
			else
			{

				double Beta;
				
				Vertex *p[20];
				p[0] = v[i];
				
				HalfEdge *hever = v[i]->most_ccw_out_halfedge();
				HalfEdge *hever1 = v[i]->most_clw_out_halfedge();
				HalfEdge *herot = hever->clw_rotate_about_source();

				int n = 2;//for even 

				while (hever1 != herot)
				{
					
					herot = herot->clw_rotate_about_source();
					n++;
				}

				Beta = 3.0000 / (float)(n + 2) / (float)n;

				Point pnt[20];
				
				pnt[0] = p[0]->point();
				for (int i = 1; i < n+1; i++)
				{
					p[i] = hever->target();
					pnt[i] = p[i]->point();
					hever = hever->clw_rotate_about_source();
				}
				double sumx = 0.0000, sumy = 0.0000, sumz = 0.0000;
				for (int i = 1; i < n+1; i++)
				{
					sumx += pnt[i][0];
					sumy += pnt[i][1];
					sumz += pnt[i][2];
				}
			
				newPnt[0] = (1.0000 - (double)(n)*Beta)*pnt[0][0] + Beta * sumx;
				newPnt[1] = (1.0000 - (double)(n)*Beta)*pnt[0][1] + Beta * sumy;
				newPnt[2] = (1.0000 - (double)(n)*Beta)*pnt[0][2] + Beta * sumz;
				
			
				

			}
			
			newv[i] = newMesh.idVertex(v[i]->id());
			newv[i]->point() = newPnt;
			
		
		}
	
	
		he = face->halfedge();
		v[0] = he->source();
		v[1] = he->target();
		temp = he->he_next();
		v[2] = temp->target();
		

		//odd add new points
		for (int i = 3; i < 6; i++)
		{

			Point tmp;
			edge = he->edge();
			int num = newMesh.numVertices();
	
		
			
			if (mesh.isBoundary(he))
			{
				
				Point src, tar;
				
				tar = v[(i - 2) % 3]->point();

				src = v[i - 3]->point();

				//Midpoint for odd points
				tmp[0] = (src[0] + tar[0]) / 2.0;
				tmp[1] = (src[1] + tar[1]) / 2.0;
				tmp[2] = (src[2] + tar[2]) / 2.0;
				
				

			}
			else
			{	
				
				HalfEdge *hetmp;
				hetmp = he->he_sym();
				hetmp = hetmp->he_next();
				
				
				Point  pnt[4];
				v[i] = hetmp->target();
				pnt[0] = v[i]->point();
				/*
				if (i == 3)
				{
					pnt[1] = v[2]->point();
					pnt[2] = v[0]->point();
					pnt[3] = v[1]->point();


				}
				else if (i == 5)
				{
					pnt[1] = v[1]->point();
					pnt[2] = v[2]->point();
					pnt[3] = v[0]->point();
				}
				else if (i == 4)
				{
					pnt[1] = v[0]->point();
					pnt[2] = v[1]->point();
					pnt[3] = v[2]->point();
				}
				*/
				pnt[1] = v[(i - 1) % 3]->point();
				pnt[2] = v[i - 3]->point();
				pnt[3] = v[(i - 2) % 3]->point();

				tmp[0] = (pnt[0][0] + pnt[1][0] + (3.0)*(pnt[2][0] + pnt[3][0])) / (8.0);
				tmp[1] = (pnt[0][1] + pnt[1][1] + (3.0)*(pnt[2][1] + pnt[3][1])) / (8.0);
				tmp[2] = (pnt[0][2] + pnt[1][2] + (3.0)*(pnt[2][2] + pnt[3][2])) / (8.0);
				
				
			}
			//check duplicates with hash map
			std::unordered_map<Edge*, Vertex*>::const_iterator got = edgemap.find(edge);
			if (got==edgemap.end()){
				
				sodel.createVertex(&newMesh, num + 1);
				v[i + 3] = newMesh.idVertex(num + 1);
				v[i + 3]->point() = tmp;
				
				edgemap.insert({ edge, v[i+3] });
			}
			else
			{
				
				v[i + 3] = got->second;
			}

			
			he = he->he_next();

		}
		
		
		

		
		//add new face
		int v1[3],v2[3],v3[3],v4[3];
		
		int numFace = newMesh.numFaces();
		
		v1[0] = v[6]->id();
		v1[1] = v[7]->id();
		v1[2] = v[8]->id();
		
	
		

		v2[0] = newv[0]->id();
		v2[1] = v[6]->id();
		v2[2] = v[8]->id();
		
		v3[0] = v[6]->id();
		v3[1] = newv[1]->id();
		v3[2] = v[7]->id();
		
		v4[0] = v[8]->id();
		v4[1] = v[7]->id();
		v4[2] = newv[2]->id();
	
		
		
		sodel.createFace(&newMesh, v1, newMesh.numFaces() + 1);
		sodel.createFace(&newMesh, v2, newMesh.numFaces() + 1);
		sodel.createFace(&newMesh, v3, newMesh.numFaces() + 1);
		sodel.createFace(&newMesh, v4, newMesh.numFaces() + 1);
		
		
	}//face end
	
	
	
	

	





















	// Write out the resultant obj file
	int vObjID = 1;
	std::map<int, int> vidToObjID;

	std::ofstream os(argv[2]);

	SolidVertexIterator iter(&newMesh);

	for(; !iter.end(); ++iter)
	{
		Vertex *v = *iter;
		Point p = v->point();
		os << "v " << p[0] << " " << p[1] << " " << p[2] << std::endl;
		vidToObjID[v->id()] = vObjID++;
	}
	os << "# " << (unsigned int)newMesh.numVertices() << " vertices" << std::endl;

	float u = 0.0, v = 0.0;
	for(iter.reset(); !iter.end(); ++iter)
	{
		Vertex *vv = *iter;
		std::string key( "uv" );
		std::string s = Trait::getTraitValue (vv->string(), key );
		if( s.length() > 0 )
		{
			sscanf( s.c_str (), "%f %f", &u, &v );
		}
		os << "vt " << u << " " << v << std::endl;
	}
	os << "# " << (unsigned int)newMesh.numVertices() << " texture coordinates" << std::endl;

	SolidFaceIterator fiter(&newMesh);
	for(; !fiter.end(); ++fiter)
	{
		Face *f = *fiter;
		FaceVertexIterator viter(f);
		os << "f " ;
		for(; !viter.end(); ++viter)
		{
			Vertex *v = *viter;
			os << vidToObjID[v->id()] << "/" << vidToObjID[v->id()] << " ";
		}
		os << std::endl;
	}
	os.close();
}