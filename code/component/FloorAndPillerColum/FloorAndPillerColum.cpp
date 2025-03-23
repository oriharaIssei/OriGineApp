#include "FloorAndPillerColum.h"



void FloorAndPillerColum::Initialize([[maybe_unused]] GameEntity* _entity) {
   
}

bool FloorAndPillerColum::Edit() {
    return false;
}

void FloorAndPillerColum::Save(BinaryWriter& _writer) {
    _writer.Write(columnHeight_);
    
}

void FloorAndPillerColum::Load(BinaryReader& _reader) {
    _reader.Read(columnHeight_);
 
}

void FloorAndPillerColum::Finalize() {}

