
#ifndef GameObject_H
#define GameObject_H

#include "d3dUtil.h"


#include "Geometry.h"
#include "constants.h"

class CanisMajor;

static int foo[1];

class Actor {
private:
	Geometry *geometry; 
	float radius;
	float radiusSquared;
	Vector3 scale;
	ID3D10EffectTechnique* mTech;
	Matrix wvp, s, t,rx,ry,rz;//world view projection
	Vector3 rotation;
protected:
	Matrix world;
	Vector3 position;
	Vector3 velocity;
	float speed;
	float health;//simple health value. Starts at MAX_HEATH at spawn and object dies if health hits 0
	//consider 100 health to be the "standard"
	float MAX_HEALTH;
	CanisMajor* game;

public:

	Actor();
	~Actor();

	void init(CanisMajor* game,Geometry *b, float r=1);
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
	virtual bool collided(Actor *gameObject);
	void virtual onDeath();
	void setRotation(Vector3 rot){rotation = rot;}
	Vector3 getRotation(){return rotation;}
	bool isActive;
	float getHealth(){return health;}
	void setHealth(float H){health = H;}

};


#endif
