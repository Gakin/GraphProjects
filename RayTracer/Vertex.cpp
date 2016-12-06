#include "Vertex.h"

float Vertex::dotProduct(Vertex a){
		return (this->coordinates[0]*a.coordinates[0]+
				this->coordinates[1]*a.coordinates[1]+
				this->coordinates[2]*a.coordinates[2]); 
	}

Vertex Vertex::crossProduct(Vertex a){
		Vertex * result = new Vertex();
		result->coordinates[0] = this->coordinates[1]*a.coordinates[2] - this->coordinates[2]*a.coordinates[1];
		result->coordinates[1] = this->coordinates[2]*a.coordinates[0] - this->coordinates[0]*a.coordinates[2];
		result->coordinates[2] = this->coordinates[0]*a.coordinates[1] - this->coordinates[1]*a.coordinates[0];
        return *result;
	}
