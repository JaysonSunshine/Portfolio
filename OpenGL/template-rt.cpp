//
// raytracer.cpp
//

#define _CRT_SECURE_NO_WARNINGS
#include "matm.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int g_width;
int g_height;

struct Ray
{
    vec4 origin;
    vec4 dir;
};

struct Sphere
{
	string name;
	float pos[3];
	float scaling[3];
	float color[3];
	float illumination[4]; //contains Ka, Kd, Ks, Kr
	float exponent; //specular exponent

};

struct Light
{
	string name;
	vec4 pos;
	float color[3];
};

vector<vec4> g_colors;

float g_left;
float g_right;
float g_top;
float g_bottom;
float g_near;
Sphere Spheres[5];
Light Lights[5];
int sphereCount = 0;
int lightCount = 0;
float background[3];
float ambient[3];
string outputName;


// -------------------------------------------------------------------
// Input file parsing

vec4 toVec4(const string& s1, const string& s2, const string& s3)
{
    stringstream ss(s1 + " " + s2 + " " + s3);
    vec4 result;
    ss >> result.x >> result.y >> result.z;
    result.w = 1.0f;
    return result;
}

float toFloat(const string& s)
{
    stringstream ss(s);
    float f;
    ss >> f;
    return f;
}

void parseLine(const vector<string>& vs)
{
	//general format is to check the first word, then fill in appropriate values for that term
	if(vs[0] == "NEAR"){
		g_near = toFloat(vs[1]); //as int?
	}
	else if(vs[0] == "LEFT"){
		g_left = toFloat(vs[1]); //as int?
	}
	else if(vs[0] == "RIGHT"){
		g_right = toFloat(vs[1]); //as int?
	}
	else if(vs[0] == "TOP"){
		g_top = toFloat(vs[1]); //as int?
	}
	else if(vs[0] == "BOTTOM"){
		g_bottom = toFloat(vs[1]); //as int?
	} 
    else if (vs[0] == "RES"){
        g_width = (int)toFloat(vs[1]);
        g_height = (int)toFloat(vs[2]);
        g_colors.resize(g_width * g_height);
    }
	else if(vs[0] == "SPHERE" && sphereCount < 5){
		Spheres[sphereCount].name = vs[1];
		for(int i = 0; i < 3; i++){
			Spheres[sphereCount].pos[i] = toFloat(vs[i + 2]);
		}
		for(int i = 0; i < 3; i++){
			Spheres[sphereCount].scaling[i] = toFloat(vs[i + 5]);
		}
		for(int i = 0; i < 3; i++){
			Spheres[sphereCount].color[i] = toFloat(vs[i + 8]);
		}
		for(int i = 0; i < 4; i++){
			Spheres[sphereCount].illumination[i] = toFloat(vs[i + 11]);
		}
		Spheres[sphereCount].exponent = toFloat(vs[15]); 
		sphereCount++;
	}
	else if(vs[0] == "LIGHT" && lightCount < 5){
		Lights[lightCount].name = vs[1];
		//for(int i = 0; i < 3; i++){
			Lights[lightCount].pos.x = toFloat(vs[2]);
			Lights[lightCount].pos.y = toFloat(vs[3]);
			Lights[lightCount].pos.z = toFloat(vs[4]);
			Lights[lightCount].pos.w = 1.0f;
		//}
		for(int i = 0; i < 3; i++){
			Lights[lightCount].color[i] = toFloat(vs[i + 5]);
		}
		lightCount++;
	}
	else if(vs[0] == "BACK"){
		for(int i = 0; i < 3; i++){
			background[i] = toFloat(vs[i + 1]);
		}
	}
	else if(vs[0] == "AMBIENT"){
		for(int i = 0; i < 3; i++){
			ambient[i] = toFloat(vs[i + 1]);
		}
	}
	else if(vs[0] == "OUTPUT"){
		outputName = vs[1];
	}
}

void loadFile(const char* filename)
{
    ifstream is(filename);
    if (is.fail())
    {
        cout << "Could not open file " << filename << endl;
        exit(1);
    }
    string s;
    vector<string> vs;
    while(!is.eof())
    {
        vs.clear();
        getline(is, s);
        istringstream iss(s);
        while (!iss.eof())
        {
            string sub;
            iss >> sub;
            vs.push_back(sub);
        }
        parseLine(vs);
    }
}


// -------------------------------------------------------------------
// Utilities

void setColor(int ix, int iy, const vec4& color)
{
    int iy2 = g_height - iy - 1; // Invert iy coordinate.
    g_colors[iy2 * g_width + ix] = color;
}


// -------------------------------------------------------------------
// Intersection routine

// TODO: add your ray-sphere intersection routine here.


// -------------------------------------------------------------------
// Ray tracing

//this function takes a point and a direction, Ray.origin and Ray.dir and determines if there is an intersection (t >= 0) along Ray.dir from Ray.origin to sphere i
//return value of t along the ray if there is an intersection, -1 if not
float intersection(int i, const Ray& ray, int depth){
	float t;
	mat4 transformation(Spheres[i].scaling[0], 0.0f, 0.0f, 0.0f,   // this matrix transforms the unit sphere into Spheres[i]
			  0.0f, Spheres[i].scaling[1], 0.0f, 0.0f,   // by column
			  0.0f, 0.0f, Spheres[i].scaling[2], 0.0f,
			  Spheres[i].pos[0], Spheres[i].pos[1], Spheres[i].pos[2], 1.0f);
	mat4 inverseTrans;
	InvertMatrix(transformation, inverseTrans);
	vec4 s_prime = inverseTrans * ray.origin;
	vec4 c_prime = inverseTrans * ray.dir;
	//values for quadratic equation
	float a = c_prime[0] * c_prime[0] + c_prime[1] * c_prime[1] + c_prime[2] * c_prime[2];
	float b = s_prime[0] * c_prime[0] + s_prime[1] * c_prime[1] + s_prime[2] * c_prime[2];
	float c = s_prime[0] * s_prime[0] + s_prime[1] * s_prime[1] + s_prime[2] * s_prime[2] - 1;
	float min_val; //1 when intersecting from origin, 0 when calculating from point on sphere
	if(depth == 0){
		min_val = 1;
	}
	else{
		min_val = 0.0001;
	}
	if((b * b - a * c) == 0){//one intersection point
		if((-b / a) >= min_val){
			t = (-b / a);
		}
		else{
			t = -1;
		}
	}
	else if((b * b - a * c) > 0){//two intersection points with sphere, find smallest t that's greater than min_val
		float t1 = (sqrt(b * b - a * c) - b) / a;
		float t2 = (-sqrt(b * b - a * c) - b) / a;
		if(t1 >= min_val && t2 >= min_val){
			t = min(t1, t2);
		}
		else if(t1 >= min_val && t2 < min_val){
			t = t1;
		}
		else if(t2 >= min_val && t1 < min_val){
			t = t2;
		}
		else{
			t = -1;
		}
	}
	else{
		t = -1;
	}
	return t;
}
//this function returns the color 
vec4 trace(const Ray& ray, int depth)
{
	if(depth == 4){//we limit to the original call and three recursive calls for calculating reflections
		return vec4(0.0f,0.0f, 0.0f, 0.0f);
	}
	float t[5] = {};
	
	for(int i = 0; i < sphereCount; i++){ //finds the possible intersection with all other 5 spheres
		t[i] = intersection(i, ray, depth);
	}
	//occlusion
	int place = 0;
	float minimum = 0;
	float min_val;
	if(depth == 0){
		min_val = 1;
	}
	else{
		min_val = 0.0001;
	}
	for(int i = 0; i < sphereCount; i++){//determines the intersection with the closet object, others being occluded
		if(minimum == 0 && t[i] >= min_val){
			minimum = length(t[i]*ray.dir);
			place = i;
		}
		if(minimum != 0 && t[i] >= min_val){
			if(length(t[i]*ray.dir) < minimum){
				minimum = length(t[i]*ray.dir);
				place = i;
			}
		}
	}
	//at this point we know which object we hit, if any
	//what point is this on unit sphere
	float diffuse[3] = {};
	float specular[3] = {};
	vec4 reflect;
	if(t[place] >= min_val){
		mat4 transformation(Spheres[place].scaling[0], 0.0f, 0.0f, 0.0f,   // column
				  0.0f, Spheres[place].scaling[1], 0.0f, 0.0f,   // column
				  0.0f, 0.0f, Spheres[place].scaling[2], 0.0f,   // column
				  Spheres[place].pos[0], Spheres[place].pos[1], Spheres[place].pos[2], 1.0f);
		mat4 inverseTrans;
		InvertMatrix(transformation, inverseTrans);
		vec4 s_prime = inverseTrans * ray.origin;
		vec4 c_prime = inverseTrans * ray.dir;
		vec4 point_intersection_normalized = s_prime + t[place]*c_prime;//point of intersection on unit sphere at origin
		vec4 point_intersection = ray.origin + t[place]*ray.dir;//point of intersection onf untransformed ellipsoid
		vec4 normal = point_intersection_normalized - vec4(0.0f, 0.0f, 0.0f, 1.0f);//normal on unit sphere
		vec3 normal_3(normal.x, normal.y, normal.z);
		mat3 inverseTrans_3(1/Spheres[place].scaling[0], 0.0f, 0.0f,
					0.0f, 1/Spheres[place].scaling[1], 0.0f,
					0.0f, 0.0f, 1/Spheres[place].scaling[2]);
		normal_3 = normalize(inverseTrans_3 * normal_3); //normal on untransformed ellipsoid
		vec4 light_vector[5];
		vec4 normal_4(normal_3.x, normal_3.y, normal_3.z, 0.0f); //normal on untransformed ellipsoid in homogenous coordinates
		for(int i = 0; i < lightCount; i++){//for each light is there a clear line of sight from this point, or is it blocked by another ellipsoid?
			light_vector[i] = normalize(Lights[i].pos - point_intersection);
			//does it interesect anything?
			Ray shadow;
			shadow.origin = point_intersection;
			shadow.dir = light_vector[i];
			float test = -1;
			for(int k = 0; k < sphereCount; k++){
				float temp = intersection(k,shadow,depth + 1);
				if(test == -1){
					test = temp;
				}
			}
			if(test == -1){//if we have a clear line of sight to the light, then calculate diffuse and specular components of local illumination
				for(int j = 0; j < 3; j++){
					vec4 r_vec = 2*(normal_4*dot(normal_4,light_vector[i])) - light_vector[i];
					vec4 view = normalize(-t[place]*ray.dir);
					if(dot(light_vector[i], normal_4) > 0){//determines if specular and diffuse light will be created
						diffuse[j] += Spheres[place].illumination[1] * Lights[i].color[j] * dot(light_vector[i], normal_4) * Spheres[place].color[j];
						if(dot(r_vec, view) > 0){ //makes sure we don't see specular highlights if we're looking at the inside of an ellipsoid
							specular[j] += Spheres[place].illumination[2] * Lights[i].color[j] * powf(dot(r_vec, view),Spheres[place].exponent);
						}
					}
				}
			}
		}
		//find reflection
		float dotprod = dot(t[place]*ray.dir, normal_4);
		vec4 reflection = normalize(t[place]*ray.dir - 2 * normal_4 *dotprod);
		Ray reflect_ray;
		reflect_ray.origin = point_intersection;
		reflect_ray.dir = reflection;
		if(dotprod < 0){
			reflect = trace(reflect_ray,depth + 1); //recursively calls trace to determine color from reflected rays off other ellipsoids
		}
		else
		{
			reflect = vec4(0.0f,0.0f,0.0f,0.0f);
		}
	}
	if(minimum != 0){ //if we hit a point, with our first ray from origin
		vec4 color;
		color.x = Spheres[place].illumination[0] * Spheres[place].color[0] * ambient[0] + diffuse[0] + specular[0] + Spheres[place].illumination[3]*reflect.x;
		//ambient + diffuse + specular + reflected ray
		if(color.x > 1){//each of these clamps the value to a maximum of 1.0
			color.x = 1;
		}
		color.y = Spheres[place].illumination[0] * Spheres[place].color[1] * ambient[1] + diffuse[1] + specular[1] + Spheres[place].illumination[3]*reflect.y;
		if(color.y > 1){
			color.y = 1;
		}
		color.z = Spheres[place].illumination[0] * Spheres[place].color[2] * ambient[2] + diffuse[2] + specular[2] + Spheres[place].illumination[3]*reflect.z;
		if(color.z > 1){
			color.z = 1;
		}
		color.w = 1.0f;
		return vec4(color.x, color.y, color.z, color.w);
	}
	else{
		if(depth == 0){//return the background if you don't hit anything on the first ray from origin
			return vec4(background[0], background[1], background[2], 1.0f);
		}
		else{//don't modify the color of the reflected ray hits nothing
			return vec4(0.0f,0.0f,0.0f,0.0f);
		}
	}
}


vec4 getDir(int ix, int iy)
{
    vec4 dir;
	float x = g_left + ((float)ix / (g_width - 1)) * (g_right - g_left);
	float y = g_bottom + ((float)iy / (g_height - 1)) * (g_top - g_bottom);
	float z = -1 * g_near;
	//float magnitude = sqrt(x * x + y * y + z * z);
    dir = normalize(vec4(x, y, z, 0));
    return dir;
}

void renderPixel(int ix, int iy)
{
    Ray ray;
    ray.origin = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    ray.dir = getDir(ix, iy);
    vec4 color = trace(ray, 0);
    setColor(ix, iy, color);
}

void render()
{
    for (int iy = 0; iy < g_height; iy++)
        for (int ix = 0; ix < g_width; ix++)
            renderPixel(ix, iy);
}


// -------------------------------------------------------------------
// PPM saving

void savePPM(int Width, int Height, const char* fname, unsigned char* pixels) 
{
    FILE *fp;
    const int maxVal=255;

    printf("Saving image %s: %d x %d\n", fname, Width, Height);
    fp = fopen(fname,"wb");
    if (!fp) {
        printf("Unable to open file '%s'\n", fname);
        return;
    }
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", Width, Height);
    fprintf(fp, "%d\n", maxVal);

    for(int j = 0; j < Height; j++) {
        fwrite(&pixels[j*Width*3], 3, Width, fp);
    }

    fclose(fp);
}

void saveFile()
{
    // Convert color components from floats to unsigned chars.
    // TODO: clamp values if out of range.
    unsigned char* buf = new unsigned char[g_width * g_height * 3];
    for (int y = 0; y < g_height; y++)
        for (int x = 0; x < g_width; x++)
            for (int i = 0; i < 3; i++)
                buf[y*g_width*3+x*3+i] = (unsigned char)(((float*)g_colors[y*g_width+x])[i] * 255.9f);	
    
    // TODO: change file name based on input file name.
	savePPM(g_width, g_height, outputName.c_str(), buf);	
    delete[] buf;
}


// -------------------------------------------------------------------
// Main

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "Usage: template-rt <input_file.txt>" << endl;
        exit(1);
    }
    loadFile(argv[1]);
    render();
    saveFile();
	return 0;
}

