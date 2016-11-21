#include <fstream>
#include <vector>

#include "OBJFileReader.h"
#include "Solid.h"
#include "iterators.h"
#include "SolidDelegate.h"
#include <unordered_map>

using namespace MeshLib;

std::unordered_map<Edge*, double> edgemap;

void main(int argc, char *argv[])
{
	// Read in the obj file
	Solid mesh;
	
	OBJFileReader of;
	std::ifstream in(argv[1]);
	of.readToSolid(&mesh, in);

	SolidDelegate sodel;
	SolidVertexIterator viter(&mesh);
	SolidFaceIterator faceiter(&mesh);
	
	Point deltaPL[2502];
	Point Normal[2502];
	Point tuettePoint[2502];
	Point harmonicPoint[2502];
	double innerProduct;
	Point normalComp;
	Point aDerv;
	for (int i = 0; i < 2502; i++)
	{
		deltaPL[i][0] = 0.0;
		deltaPL[i][1] = 0.0;
		deltaPL[i][2] = 0.0;
		Normal[i][0] = 0.0;
		Normal[i][1] = 0.0;
		Normal[i][2] = 0.0;
		tuettePoint[i][0] = 0.0;
		tuettePoint[i][1] = 0.0;
		tuettePoint[i][2] = 0.0;
		harmonicPoint[i] = { 0.0, 0.0, 0.0 };

	}
	for (; !viter.end(); ++viter)
	{
		
		Vertex *vertmp = *viter;
		//VertexOutHalfedgeIterator voh(&mesh, vertmp);
		
		Point norm;//calc new Norm
		double totalArea = 0.0;
		
		norm[0] = norm[1] = norm[2] = 0.0;
		VertexFaceIterator vf(vertmp);
		for (; !vf.end(); ++vf){
			double area = 0.0;
			Face *f = *vf;
			
			Point v1 = f->halfedge()->source()->point();
			Point v2 = f->halfedge()->target()->point();
			Point v3 = f->halfedge()->he_next()->target()->point();

			area = ((v3 - v1) ^ (v2 - v1)).norm();
			

			Point n = f->norm();
			norm += n * area;
			totalArea += area;
		}
		

		Point newNorm;
		norm = norm / totalArea;
		double normN = norm.norm();
		newNorm = norm / normN;
		

		//Normal & Gauss Map
		Normal[vertmp->id() - 1] = newNorm;
	
		

	}
	
	SolidEdgeIterator edgeiter(&mesh);

	
	double tuetteEnergy = 0.0;
	double newTEnergy = 0.0;
	double harmoincEnergy = 0.0;
	double newHEnergy = 0.0;
	double stepLength = 0.01;
	double eThreshold = 0.00001;
	
	for (; !edgeiter.end(); ++edgeiter) {
		Edge *e = *edgeiter;

		Vertex *s = mesh.edgeVertex1(e);
		Vertex *t = mesh.edgeVertex2(e);
	
		tuetteEnergy += (Normal[s->id() - 1] - Normal[t->id() - 1]).norm2();
		 
		
	}

	//harmonic K
	double K, cota, cotb;


	edgeiter.reset();
	
	for (; !edgeiter.end(); ++edgeiter) {
		Edge *e = *edgeiter;
		Vertex *s = mesh.edgeVertex1(e);
		Vertex *t = mesh.edgeVertex2(e);
		HalfEdge *he = e->halfedge(1);
		he = he->ccw_rotate_about_source();
		Vertex *v1 = he->target();
		he = he->clw_rotate_about_source()->clw_rotate_about_source();
		Vertex *v2 = he->target();
		cota = (s->point() - v1->point())*(t->point() - v1->point()) / ((s->point() - v1->point()) ^ (t->point() - v1->point())).norm();
		cotb = (s->point() - v2->point())*(t->point() - v2->point()) / ((s->point() - v2->point()) ^ (t->point() - v2->point())).norm();
		
		
		K = (cota + cotb) / 2.0;
		edgemap.insert({ e, K });

	}

	viter.reset();
	//Gauss map update
	for (; !viter.end(); ++viter) {
		Vertex *vertmp = *viter;
		vertmp->point() = Normal[vertmp->id() - 1]  ;
	}

	
	
	viter.reset();
	

	for (; !viter.end(); ++viter)//step2
	{


		Vertex *vertmp = *viter;
		VertexOutHalfedgeIterator voh(&mesh, vertmp);
		Point u = vertmp->point();
		int id = vertmp->id()-1;
		double num = 0.0;
		for (; !voh.end(); ++voh)
		{
			
			HalfEdge *he1 = *voh;
			Vertex *v1 = he1->target();
			Point v = v1->point();

			deltaPL[id] += v - u;
			num = num + 1.0;
		}
		deltaPL[id] = deltaPL[id] / num;
	}

	//absolute derivatives
	viter.reset();
	for (; !viter.end(); ++viter)//step2
	{
		Vertex *vertmp = *viter;
		VertexOutHalfedgeIterator voh(&mesh, vertmp);
		Point u = vertmp->point();
		int id = vertmp->id() - 1;
		
		innerProduct = deltaPL[id] * Normal[id];
	
	
		normalComp[0] = innerProduct * Normal[id][0];
		normalComp[1] = innerProduct * Normal[id][1];
		normalComp[2] = innerProduct * Normal[id][2];
		
		Point aDerv = deltaPL[id] - normalComp;
		Point tuetteUpdate = aDerv * stepLength;
		

		tuettePoint[id] = tuetteUpdate; //step 3
		
	}

	viter.reset();
	for (; !viter.end(); ++viter)//step2
	{
		Vertex *vertmp = *viter;
		int id = vertmp->id() - 1;
		Normal[id] += tuettePoint[id];
		Normal[id] = Normal[id] / Normal[id].norm();
	}

	
		
	edgeiter.reset();
		
	for (; !edgeiter.end(); ++edgeiter) {
		Edge *e = *edgeiter;

		Vertex *s = mesh.edgeVertex1(e);
		Vertex *t = mesh.edgeVertex2(e);

		newTEnergy += (Normal[s->id() - 1] - Normal[t->id() - 1]).norm2();

   	}
	viter.reset();
		//tuette map update
	for (; !viter.end(); ++viter) {
		Vertex *vertmp = *viter;
		vertmp->point() =Normal[vertmp->id() - 1];
	}

	
	
	
	
	//while(1==0)
	while (!(fabs(newTEnergy - tuetteEnergy) < eThreshold*1000))
	{//step 5
		tuetteEnergy = newTEnergy;
		newTEnergy = 0.0;
		//printf("%f\n", tuetteEnergy);
		
		
		
		
		viter.reset();


		for (; !viter.end(); ++viter)//step2
		{


			Vertex *vertmp = *viter;
			VertexOutHalfedgeIterator voh(&mesh, vertmp);
			Point u = vertmp->point();
			int id = vertmp->id() - 1;
			double num = 0.0;
			for (; !voh.end(); ++voh)
			{

				HalfEdge *he1 = *voh;
				Vertex *v1 = he1->target();
				Point v = v1->point();

				deltaPL[id] += v - u;
				num = num + 1.0;
			}
			deltaPL[id] = deltaPL[id] / num;
		}

		viter.reset();
		for (; !viter.end(); ++viter)//step2
		{
			Vertex *vertmp = *viter;
			VertexOutHalfedgeIterator voh(&mesh, vertmp);
			Point u = vertmp->point();
			int id = vertmp->id() - 1;

			innerProduct = deltaPL[id] * Normal[id];


			normalComp[0] = innerProduct * Normal[id][0];
			normalComp[1] = innerProduct * Normal[id][1];
			normalComp[2] = innerProduct * Normal[id][2];

			Point aDerv = deltaPL[id] - normalComp;
			Point tuetteUpdate = aDerv * stepLength;


			tuettePoint[id] = tuetteUpdate; //step 3

		}

		viter.reset();
		for (; !viter.end(); ++viter)//step2
		{
			Vertex *vertmp = *viter;
			int id = vertmp->id() - 1;
			Normal[id] += tuettePoint[id];
			Normal[id] = Normal[id] / Normal[id].norm();
		}



		edgeiter.reset();

		for (; !edgeiter.end(); ++edgeiter) {
			Edge *e = *edgeiter;

			Vertex *s = mesh.edgeVertex1(e);
			Vertex *t = mesh.edgeVertex2(e);

			newTEnergy += (Normal[s->id() - 1] - Normal[t->id() - 1]).norm2();

		}
		viter.reset();
		//tuette map update
		for (; !viter.end(); ++viter) {
			Vertex *vertmp = *viter;
			vertmp->point() = Normal[vertmp->id() - 1];
		}
}

	tuetteEnergy = newTEnergy;
	harmoincEnergy = tuetteEnergy;
	//harmonic

	viter.reset();


	for (; !viter.end(); ++viter)//step2
	{


		Vertex *vertmp = *viter;
		VertexOutHalfedgeIterator voh(&mesh, vertmp);
		Point u = vertmp->point();
		int id = vertmp->id() - 1;
		double num = 0.0;
		for (; !voh.end(); ++voh)
		{

			HalfEdge *he1 = *voh;
			Edge *e = he1->edge();
			Vertex *v1 = he1->target();
			Point v = v1->point();
			std::unordered_map<Edge*,double>::const_iterator got = edgemap.find(e);
			double K = got->second;
			deltaPL[id] += (v - u) * K;
			num = num + 1.0;
		}
		deltaPL[id] = deltaPL[id] / num;
	}
	
	//absolute derivatives
	viter.reset();
	for (; !viter.end(); ++viter)//step2
	{
		Vertex *vertmp = *viter;
		VertexOutHalfedgeIterator voh(&mesh, vertmp);
		Point u = vertmp->point();
		int id = vertmp->id() - 1;

		innerProduct = deltaPL[id] * Normal[id];


		normalComp[0] = innerProduct * Normal[id][0];
		normalComp[1] = innerProduct * Normal[id][1];
		normalComp[2] = innerProduct * Normal[id][2];

		Point aDerv = deltaPL[id] - normalComp;
		Point hUpdate = aDerv * stepLength;


		harmonicPoint[id] =hUpdate; //step 3

	}
	// compute mass center
	Point mass = { 0.0, 0.0, 0.0 };
	viter.reset();

	for (; !viter.end(); ++viter)//step2
	{
		Vertex *vertmp = *viter;
		mass += vertmp->point();
	}
	mass = mass / 2502;

	// h update w/ mass
	viter.reset();
	for (; !viter.end(); ++viter)//step2
	{
		Vertex *vertmp = *viter;
		int id = vertmp->id() - 1;
		Normal[id] +=harmonicPoint[id];
		Normal[id] -= mass;
		Normal[id] = Normal[id] / Normal[id].norm();
	}

	//h energy
	edgeiter.reset();

	for (; !edgeiter.end(); ++edgeiter) {
		Edge *e = *edgeiter;

		Vertex *s = mesh.edgeVertex1(e);
		Vertex *t = mesh.edgeVertex2(e);
		
		std::unordered_map<Edge*, double>::const_iterator got = edgemap.find(e);
		double K = got->second;
		
		newHEnergy += (Normal[s->id() - 1] - Normal[t->id() - 1]).norm2()*K;

	}
	viter.reset();
	//h map update
	for (; !viter.end(); ++viter) {
		Vertex *vertmp = *viter;
		vertmp->point() = Normal[vertmp->id() - 1];
	}
	
	
	
	while (!(fabs(newHEnergy - harmoincEnergy) < eThreshold))
	{
		harmoincEnergy = newHEnergy;
		newHEnergy = 0.0;
		//printf("%f\n", harmoincEnergy);


		viter.reset();


		for (; !viter.end(); ++viter)//step2
		{


			Vertex *vertmp = *viter;
			VertexOutHalfedgeIterator voh(&mesh, vertmp);
			Point u = vertmp->point();
			int id = vertmp->id() - 1;
			double num = 0.0;
			for (; !voh.end(); ++voh)
			{

				HalfEdge *he1 = *voh;
				Edge *e = he1->edge();
				Vertex *v1 = he1->target();
				Point v = v1->point();
				std::unordered_map<Edge*, double>::const_iterator got = edgemap.find(e);
				double K = got->second;
				deltaPL[id] += (v - u) * K;
				num = num + 1.0;
			}
			deltaPL[id] = deltaPL[id] / num;
		}

		//absolute derivatives
		viter.reset();
		for (; !viter.end(); ++viter)//step2
		{
			Vertex *vertmp = *viter;
			VertexOutHalfedgeIterator voh(&mesh, vertmp);
			Point u = vertmp->point();
			int id = vertmp->id() - 1;

			innerProduct = deltaPL[id] * Normal[id];


			normalComp[0] = innerProduct * Normal[id][0];
			normalComp[1] = innerProduct * Normal[id][1];
			normalComp[2] = innerProduct * Normal[id][2];

			Point aDerv = deltaPL[id] - normalComp;
			Point hUpdate = aDerv * stepLength;


			harmonicPoint[id] = hUpdate; //step 3

		}
		// compute mass center
		Point mass = { 0.0, 0.0, 0.0 };
		viter.reset();

		for (; !viter.end(); ++viter)//step2
		{
			Vertex *vertmp = *viter;
			mass += vertmp->point();
		}
		mass = mass / 2502;

		// h update w/ mass
		viter.reset();
		for (; !viter.end(); ++viter)//step2
		{
			Vertex *vertmp = *viter;
			int id = vertmp->id() - 1;
			Normal[id] += harmonicPoint[id];
			Normal[id] -= mass;
			Normal[id] = Normal[id] / Normal[id].norm();
		}

		//h energy
		edgeiter.reset();

		for (; !edgeiter.end(); ++edgeiter) {
			Edge *e = *edgeiter;

			Vertex *s = mesh.edgeVertex1(e);
			Vertex *t = mesh.edgeVertex2(e);

			std::unordered_map<Edge*, double>::const_iterator got = edgemap.find(e);
			double K = got->second;

			newHEnergy += (Normal[s->id() - 1] - Normal[t->id() - 1]).norm2()*K;

		}
		viter.reset();
		//h map update
		for (; !viter.end(); ++viter) {
			Vertex *vertmp = *viter;
			vertmp->point() = Normal[vertmp->id() - 1];
		}

	}

	/***conformal****/


	// Write out the resultant obj file
	int vObjID = 1;
	std::map<int, int> vidToObjID;

	std::ofstream os(argv[2]);

	SolidVertexIterator iter(&mesh);

	for (; !iter.end(); ++iter)
	{
		Vertex *v = *iter;
		Point p = v->point();
		os << "v " << p[0] << " " << p[1] << " " << p[2] << std::endl;
		vidToObjID[v->id()] = vObjID++;
	}
	os << "# " << (unsigned int)mesh.numVertices() << " vertices" << std::endl;


	//texture
	float u = 0.0, v = 0.0;
	for (iter.reset(); !iter.end(); ++iter)
	{
		Vertex *vv = *iter;
		std::string key("uv");
		std::string s = Trait::getTraitValue(vv->string(), key);
		if (s.length() > 0)
		{
			sscanf(s.c_str(), "%f %f", &u, &v);
		}
		os << "vt " << u << " " << v << std::endl;
	}
	os << "# " << (unsigned int)mesh.numVertices() << " texture coordinates" << std::endl;

	SolidFaceIterator fiter(&mesh);
	for (; !fiter.end(); ++fiter)
	{
		Face *f = *fiter;
		FaceVertexIterator viter(f);
		os << "f ";
		for (; !viter.end(); ++viter)
		{
			Vertex *v = *viter;
			os << vidToObjID[v->id()] << "/" << vidToObjID[v->id()] << " ";
		}
		os << std::endl;
	}
	os.close();
}