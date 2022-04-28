#pragma once
#include"stdafx.h"
#include"../Common/CollisionPrimitive.h"
namespace ButiEngine {

namespace Collision {
	const char MaxLevel = 7;

	template<typename T>
	class OctCell;
	template<typename T>
	class OctRegistObj;

	template<typename T>
	struct CollisionObject :public IObject {

		CollisionObject(Value_weak_ptr<CollisionPrimitive> arg_vwp_coliisionPrim, Value_weak_ptr<T> arg_vwp_obj) {
			vwp_coliisionPrim = arg_vwp_coliisionPrim;
			vwp_obj = arg_vwp_obj;
		}
		~CollisionObject() {

		}
		void Initialize()override {}
		void PreInitialize()override {}
		Value_weak_ptr<CollisionPrimitive> vwp_coliisionPrim;
		Value_weak_ptr<T> vwp_obj;
	};

	using CollisionObject_GameObject = CollisionObject<GameObject>;
	using CollisionObject_DrawObject = CollisionObject<ButiRendering::IDrawObject>;

	template<typename T>
	class OctRegistObj:public enable_value_from_this<OctRegistObj<T>>
	{
	public:
		inline OctRegistObj(Value_ptr<CollisionObject <T>> arg_vlp_collisionObj) {

			vlp_collisionObject = arg_vlp_collisionObj;

		}
		~OctRegistObj() {
			vlp_collisionObject = nullptr;
		}
		OctCell<T>* p_cell=nullptr;		
		Value_ptr<CollisionObject< T>> vlp_collisionObject;				
		Value_ptr<OctRegistObj<T>> vlp_next=nullptr;	
		Value_ptr<OctRegistObj<T>> vlp_bef=nullptr;	

		bool Remove()
		{
			if (!p_cell)
				return false;


			if (vlp_next)
			{
				vlp_next->vlp_bef = vlp_bef;
			}
			if (vlp_bef)
			{
				vlp_bef->vlp_next = vlp_next;
			}
			p_cell->OnRemove(this);
			vlp_next = nullptr;
			vlp_bef = nullptr;
			p_cell = nullptr;
			return true;
		}

		void RegistCell(OctCell<T>* arg_pCell)
		{
			p_cell = arg_pCell;
		}

		Value_ptr<OctRegistObj<T>> GetBefObj() {
			return vlp_bef;
		}
	};


	template<typename T>
	class OctCell {
	public:
		OctCell() {

		}
		~OctCell() {
			while (vlp_head&&vlp_head->vlp_next)
			{
				vlp_head = vlp_head->vlp_next;
				vlp_head->vlp_bef = nullptr;
			}
			vlp_head = nullptr;
		}
		inline void RegistObject(Value_ptr<OctRegistObj< T>> arg_obj) {

			if (arg_obj->p_cell == this||!arg_obj)
				return;

			arg_obj->Remove();

			arg_obj->p_cell = this;

			if (vlp_head) {
				arg_obj->vlp_next = vlp_head;
				vlp_head->vlp_bef = arg_obj;
				vlp_head = arg_obj;
			}

			vlp_head = arg_obj;
		}
		void OnRemove(OctRegistObj< T>* arg_remove)
		{
			if ((vlp_head.get() ==arg_remove )&& arg_remove) {
				vlp_head = vlp_head->vlp_next;
			}
		}
		Value_ptr< OctRegistObj<T>>& GetHead() {
			return vlp_head;
		}
	private:
		Value_ptr< OctRegistObj< T>> vlp_head;

	};

	template<typename T>
	class CollisionLayer :public IObject
	{
	public:
		CollisionLayer(const unsigned char  arg_level,const Vector3& arg_minPos,const Vector3& arg_maxPos) {

			if (arg_level <= MaxLevel)
				maxLevel = arg_level;
			else {
				maxLevel = MaxLevel;
			}
			rangeMax = arg_maxPos;
			rangeMin = arg_minPos;
			width = rangeMax - rangeMin;
			OctPow[0] = 1;
			OctPowSevenDevided[0] = 0;
			for (std::int32_t i = 1; i < MaxLevel + 1; i++) {
				OctPow[i] = OctPow[i - 1] * 8;
				OctPowSevenDevided[i] = (OctPow[i] - 1) / 7;
			}
			maxCellNum = OctPowSevenDevided[MaxLevel - 1];

			unit = width / ((float)(1 << maxLevel));
			ary_cells = new OctCell<T> * [maxCellNum]();

			CreateCell(0);

		}
		~CollisionLayer() {
			vec_vlp_collisionObjs.clear();
			for (auto itr = vec_index.begin(); itr != vec_index.end(); itr++) {
				delete (*itr);
			}
			for (std::uint32_t i = 0; i < maxCellNum; i++) {
				if (ary_cells[i] )
					delete ary_cells[i];
			}
			delete[] ary_cells;
		}
		void RegistCollisionObj(Value_ptr< CollisionPrimitive> arg_primitive, Value_ptr< T> arg_collisionObj) {

			std::uint32_t* index = new std::uint32_t(vec_vlp_collisionObjs.size());

			vec_vlp_collisionObjs.push_back(make_value<OctRegistObj<T>>(ObjectFactory::Create<CollisionObject<T>>(arg_primitive, arg_collisionObj)));

			vec_index.push_back(index);
			map_objIndex.emplace(arg_collisionObj, index);
		}
		void UnRegistCollisionObj(Value_ptr< T>  arg_index) {
			if (!map_objIndex.count(arg_index)) {
				return;
			}
			auto index = *map_objIndex.at(arg_index);
			if (index >= vec_vlp_collisionObjs.size()) {
				return;
			}
			auto itr = vec_vlp_collisionObjs.begin();
			itr += index;
			(*itr)->Remove();
			vec_vlp_collisionObjs.erase(itr);

			delete map_objIndex.at(arg_index);
			auto numItr = vec_index.begin() + index;
			numItr = vec_index.erase(numItr);

			for (; numItr != vec_index.end(); numItr++) {
				*(*numItr) -= 1;
			}
		}
		void Initialize()override {
		}
		void PreInitialize()override{}
		void Update() {


			RegistOctree();
		}
		inline void RegistOctree() {
			Vector3 minPoint, maxPoint;
			for (auto itr = vec_vlp_collisionObjs.begin(); itr != vec_vlp_collisionObjs.end(); itr++) {
				(*itr)->vlp_collisionObject-> vwp_coliisionPrim.lock()->GetMaxPointAndMinPoint(maxPoint, minPoint);
				auto cellNum = GetMortonSpace(minPoint, maxPoint);

				if (cellNum > maxCellNum) {
					continue;
				}
				if (!ary_cells[cellNum]) {
					CreateCell(cellNum);
				}
				ary_cells[cellNum]->RegistObject(*itr);
			}
		}
		virtual void Check(std::vector<Value_ptr<CollisionObject< T>>>& vec_collisionObjects) {
			std::list<Value_ptr<CollisionObject< T>>> list_objStack;


			CreateCollisionObjectList(0, vec_collisionObjects, list_objStack);

		}

		void Check(Value_ptr<CollisionPrimitive> arg_vlp_checkPrimitive, std::vector<Value_ptr<CollisionObject< T>>>& arg_vec_collisionObjects) {

			Vector3 minPoint, maxPoint;

			arg_vlp_checkPrimitive->GetMaxPointAndMinPoint(maxPoint, minPoint);
			auto cellNum = GetMortonSpace(minPoint, maxPoint);
			if (!ary_cells[cellNum] && cellNum < maxCellNum) {
				CreateCell(cellNum);
			}
			std::vector<Value_ptr<CollisionObject< T>>> vec_collisionObjects;
			CreateObjectList(cellNum, vec_collisionObjects);
			auto endItr = vec_collisionObjects.end();
			for (auto itr = vec_collisionObjects.begin(); itr != endItr; itr++) {
				if ((*itr)->vwp_coliisionPrim.lock()->IsHit(arg_vlp_checkPrimitive)) {
					arg_vec_collisionObjects.push_back((*itr));
				}
			}

		}
		void Check(Value_ptr<CollisionPrimitive> arg_vlp_checkPrimitive, std::vector<Value_ptr< T>>& arg_vec_collisionObjects) {

			Vector3 minPoint, maxPoint;

			arg_vlp_checkPrimitive->GetMaxPointAndMinPoint(maxPoint, minPoint);
			auto cellNum = GetMortonSpace(minPoint, maxPoint);
			if (!ary_cells[cellNum] && cellNum < maxCellNum) {
				CreateCell(cellNum);
			}
			std::vector<Value_ptr<CollisionObject< T>>> vec_collisionObjects;
			CreateObjectList(cellNum, vec_collisionObjects);
			auto endItr = vec_collisionObjects.end();
			for (auto itr = vec_collisionObjects.begin(); itr != endItr; itr++) {
				if ((*itr)->vwp_coliisionPrim.lock()->IsHit(arg_vlp_checkPrimitive)) {
					assert(0 && "修正");
					//arg_vec_collisionObjects.push_back((*itr)->vwp_obj.lock());
				}
			}

		}
		std::vector<Value_ptr< OctRegistObj<T>>>& GetObjects() {
			return vec_vlp_collisionObjs;
		}

	private:
		inline DWORD  Get3DMortonNumber(const unsigned char  x,const unsigned char  y,const unsigned char  z)
		{
			return OctreeHelper::BitSeparate(x) | OctreeHelper::BitSeparate(y) << 1 | OctreeHelper::BitSeparate(z) << 2;
		}
		inline DWORD  Get3DMortonNumber(const Vector3& arg_position)
		{
			return Get3DMortonNumber(
				(unsigned char)((arg_position.x - rangeMin.x) / unit.x),
				(unsigned char)((arg_position.y - rangeMin.y) / unit.y),
				(unsigned char)((arg_position.z - rangeMin.z) / unit.z));
		}
		inline std::uint16_t GetMortonSpace(const Vector3& arg_minPos, const Vector3& arg_maxPos) {
			if (arg_minPos == Vector3Const::Zero && arg_maxPos == Vector3Const::Zero) {
				return 0;
			}
			auto maxSpace = Get3DMortonNumber(arg_maxPos );
			auto levelCheckFlag = Get3DMortonNumber(arg_minPos) ^maxSpace;
			auto level = OctreeHelper::GetLevel(levelCheckFlag,maxLevel);
			auto num= (maxSpace >> ((level)*3 ));
			
			num += OctPowSevenDevided[maxLevel - level];
			return (std::uint16_t)num;
		}
		inline void CreateCollisionObjectList(const std::uint16_t arg_cellNum, std::vector<Value_ptr< CollisionObject<T>>>& arg_output, std::list<Value_ptr<  CollisionObject<T>>>& arg_stack) {

			auto objItr = ary_cells[arg_cellNum]->GetHead();
			while (objItr)
			{
				auto rObjItr = objItr->vlp_next;
				while (rObjItr != nullptr) {
					arg_output.push_back(objItr->vlp_collisionObject);
					arg_output.push_back(rObjItr->vlp_collisionObject);

					rObjItr = rObjItr->vlp_next;
				}
				//  衝突スタックとの衝突リスト作成
				for (auto itr = arg_stack.begin(); itr != arg_stack.end(); itr++) {
					arg_output.push_back(objItr->vlp_collisionObject);
					arg_output.push_back(*itr);
				}
				objItr = objItr->vlp_next;
			}

			bool ChildFlag = false;
			//  子空間に移動
			DWORD ObjNum = 0;
			DWORD i, nextCellNum;
			for (i = 0; i < 8; i++) {
				nextCellNum = arg_cellNum * 8 + 1 + i;
				if (nextCellNum < maxCellNum && ary_cells[arg_cellNum * 8 + 1 + i]) {
					if (!ChildFlag) {
						objItr = ary_cells[arg_cellNum]->GetHead();
						while (objItr) {
							arg_stack.push_back(objItr->vlp_collisionObject);
							ObjNum++;
							objItr = objItr->vlp_next;
						}
					}
					ChildFlag = true;
					CreateCollisionObjectList(arg_cellNum * 8 + 1 + i, arg_output, arg_stack);
				}
			}

			if (ChildFlag) {
				for (i = 0; i < ObjNum; i++)
					arg_stack.pop_back();
			}

		}
		inline void CreateObjectList(const std::uint16_t arg_cellNum, std::vector<Value_ptr<  CollisionObject<T>>>& arg_output) {

			auto objItr = ary_cells[arg_cellNum]->GetHead();
			while (objItr)
			{
				arg_output.push_back(objItr->vlp_collisionObject);
				
				objItr = objItr->vlp_next;
			}

			//  子空間に移動
			DWORD ObjNum = 0;
			DWORD i, nextCellNum;
			for (i = 0; i < 8; i++) {
				nextCellNum = arg_cellNum * 8 + 1 + i;
				if (nextCellNum < maxCellNum && ary_cells[arg_cellNum * 8 + 1 + i]) {
					
					CreateObjectList(arg_cellNum * 8 + 1 + i, arg_output);
				}
			}


		}
		inline void CreateCell(std::uint16_t arg_cellNum) {

			while (!ary_cells[arg_cellNum])
			{
				ary_cells[arg_cellNum]= new OctCell<T>();

				// 親空間へ移動
				arg_cellNum = (arg_cellNum - 1) >> 3;
				if (arg_cellNum >=maxCellNum) break;
			}

		}
		std::vector<Value_ptr< OctRegistObj<T>>> vec_vlp_collisionObjs;


		std::unordered_map<Value_ptr<T>, std::uint32_t*> map_objIndex;
		std::vector<std::uint32_t*>vec_index;
		OctCell<T>** ary_cells;
		std::uint32_t OctPow[MaxLevel + 1];
		std::uint32_t OctPowSevenDevided[MaxLevel + 1];
		Vector3 width;	
		Vector3 rangeMin;	
		Vector3 rangeMax;	
		Vector3 unit;		
		DWORD maxCellNum;	
		unsigned char maxLevel;
		bool isCheckHitSame;
	};

	template<typename T>
	class CollisionLayer_noCheckSame :public CollisionLayer<T> {
	public:
		CollisionLayer_noCheckSame(const unsigned char  arg_level, const Vector3& arg_minPos, const Vector3& arg_maxPos):CollisionLayer<T>(arg_level,arg_minPos,arg_maxPos){

		}
		void Check(std::vector<Value_ptr<CollisionObject< T>>>& vec_collisionObjects) override{

		}
	};
}
}