
#ifndef GameObject_H
#define GameObject_H

#include "d3dUtil.h"


#include "Geometry.h"
#include "constants.h"

class NuclearLiberation;

static int foo[1];

class Actor
{
public:

	Actor();
	~Actor();

	void init(NuclearLiberation* game,Geometry *b, float r);
	void draw(ID3D10EffectMatrixVariable* fx, Matrix& camera, Matrix& projection, ID3D10EffectTechnique* mTech);
	void update(float dt);
	void create(Vector3 pos);

	void setPosition (Vector3 pos) {position = pos;}
	Vector3 getPosition() {return position;}
	void setVelocity (Vector3 vel) {velocity = vel;}
	Vector3 getVelocity() {return velocity;}
	void setSpeed(float s) {speed = s;}
	float getSpeed() {return speed;}
	void setRadius(float r) {radius = r; radiusSquared = (getSmallestScale()*r)*(getSmallestScale()*r);}
	float getRadiusSquare() {return radiusSquared;}
	float getRadius() {return radius;}
	Matrix getWorldMatrix() {return world;}
	void setScale(Vector3 s) {scale = s; radiusSquared = (getSmallestScale()*radius)*(getSmallestScale()*radius);}
	Vector3 getScale() {return scale;}
	float getLargestScale(){return max(max(scale.x,scale.y),scale.z);}
	float getSmallestScale(){return min(min(scale.x,scale.y),scale.z);}
	void setMTech(ID3D10EffectTechnique* m){ mTech = m;}
	bool collided(Actor *gameObject);

	bool isActive;

private:
	Geometry *geometry; 
	float radius;
	float radiusSquared;
	Vector3 scale;
	ID3D10EffectTechnique* mTech;
	Matrix wvp, w1,w2;//world view projection

protected:
	Matrix world;
	Vector3 position;
	Vector3 velocity;
	float speed;
	NuclearLiberation* game;
};


#endif
