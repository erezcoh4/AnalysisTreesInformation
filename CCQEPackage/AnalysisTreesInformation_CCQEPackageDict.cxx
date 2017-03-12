// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME AnalysisTreesInformation_CCQEPackageDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "AnalyzeVertex.h"
#include "calcEventTopologies.h"
#include "myVertex.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *calcEventTopologies_Dictionary();
   static void calcEventTopologies_TClassManip(TClass*);
   static void *new_calcEventTopologies(void *p = 0);
   static void *newArray_calcEventTopologies(Long_t size, void *p);
   static void delete_calcEventTopologies(void *p);
   static void deleteArray_calcEventTopologies(void *p);
   static void destruct_calcEventTopologies(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::calcEventTopologies*)
   {
      ::calcEventTopologies *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::calcEventTopologies));
      static ::ROOT::TGenericClassInfo 
         instance("calcEventTopologies", "calcEventTopologies.h", 33,
                  typeid(::calcEventTopologies), DefineBehavior(ptr, ptr),
                  &calcEventTopologies_Dictionary, isa_proxy, 4,
                  sizeof(::calcEventTopologies) );
      instance.SetNew(&new_calcEventTopologies);
      instance.SetNewArray(&newArray_calcEventTopologies);
      instance.SetDelete(&delete_calcEventTopologies);
      instance.SetDeleteArray(&deleteArray_calcEventTopologies);
      instance.SetDestructor(&destruct_calcEventTopologies);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::calcEventTopologies*)
   {
      return GenerateInitInstanceLocal((::calcEventTopologies*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::calcEventTopologies*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *calcEventTopologies_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::calcEventTopologies*)0x0)->GetClass();
      calcEventTopologies_TClassManip(theClass);
   return theClass;
   }

   static void calcEventTopologies_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *myVertex_Dictionary();
   static void myVertex_TClassManip(TClass*);
   static void *new_myVertex(void *p = 0);
   static void *newArray_myVertex(Long_t size, void *p);
   static void delete_myVertex(void *p);
   static void deleteArray_myVertex(void *p);
   static void destruct_myVertex(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::myVertex*)
   {
      ::myVertex *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::myVertex));
      static ::ROOT::TGenericClassInfo 
         instance("myVertex", "myVertex.h", 31,
                  typeid(::myVertex), DefineBehavior(ptr, ptr),
                  &myVertex_Dictionary, isa_proxy, 4,
                  sizeof(::myVertex) );
      instance.SetNew(&new_myVertex);
      instance.SetNewArray(&newArray_myVertex);
      instance.SetDelete(&delete_myVertex);
      instance.SetDeleteArray(&deleteArray_myVertex);
      instance.SetDestructor(&destruct_myVertex);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::myVertex*)
   {
      return GenerateInitInstanceLocal((::myVertex*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::myVertex*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *myVertex_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::myVertex*)0x0)->GetClass();
      myVertex_TClassManip(theClass);
   return theClass;
   }

   static void myVertex_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *AnalyzeVertex_Dictionary();
   static void AnalyzeVertex_TClassManip(TClass*);
   static void *new_AnalyzeVertex(void *p = 0);
   static void *newArray_AnalyzeVertex(Long_t size, void *p);
   static void delete_AnalyzeVertex(void *p);
   static void deleteArray_AnalyzeVertex(void *p);
   static void destruct_AnalyzeVertex(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AnalyzeVertex*)
   {
      ::AnalyzeVertex *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::AnalyzeVertex));
      static ::ROOT::TGenericClassInfo 
         instance("AnalyzeVertex", "AnalyzeVertex.h", 31,
                  typeid(::AnalyzeVertex), DefineBehavior(ptr, ptr),
                  &AnalyzeVertex_Dictionary, isa_proxy, 4,
                  sizeof(::AnalyzeVertex) );
      instance.SetNew(&new_AnalyzeVertex);
      instance.SetNewArray(&newArray_AnalyzeVertex);
      instance.SetDelete(&delete_AnalyzeVertex);
      instance.SetDeleteArray(&deleteArray_AnalyzeVertex);
      instance.SetDestructor(&destruct_AnalyzeVertex);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AnalyzeVertex*)
   {
      return GenerateInitInstanceLocal((::AnalyzeVertex*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::AnalyzeVertex*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AnalyzeVertex_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::AnalyzeVertex*)0x0)->GetClass();
      AnalyzeVertex_TClassManip(theClass);
   return theClass;
   }

   static void AnalyzeVertex_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_calcEventTopologies(void *p) {
      return  p ? new(p) ::calcEventTopologies : new ::calcEventTopologies;
   }
   static void *newArray_calcEventTopologies(Long_t nElements, void *p) {
      return p ? new(p) ::calcEventTopologies[nElements] : new ::calcEventTopologies[nElements];
   }
   // Wrapper around operator delete
   static void delete_calcEventTopologies(void *p) {
      delete ((::calcEventTopologies*)p);
   }
   static void deleteArray_calcEventTopologies(void *p) {
      delete [] ((::calcEventTopologies*)p);
   }
   static void destruct_calcEventTopologies(void *p) {
      typedef ::calcEventTopologies current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::calcEventTopologies

namespace ROOT {
   // Wrappers around operator new
   static void *new_myVertex(void *p) {
      return  p ? new(p) ::myVertex : new ::myVertex;
   }
   static void *newArray_myVertex(Long_t nElements, void *p) {
      return p ? new(p) ::myVertex[nElements] : new ::myVertex[nElements];
   }
   // Wrapper around operator delete
   static void delete_myVertex(void *p) {
      delete ((::myVertex*)p);
   }
   static void deleteArray_myVertex(void *p) {
      delete [] ((::myVertex*)p);
   }
   static void destruct_myVertex(void *p) {
      typedef ::myVertex current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::myVertex

namespace ROOT {
   // Wrappers around operator new
   static void *new_AnalyzeVertex(void *p) {
      return  p ? new(p) ::AnalyzeVertex : new ::AnalyzeVertex;
   }
   static void *newArray_AnalyzeVertex(Long_t nElements, void *p) {
      return p ? new(p) ::AnalyzeVertex[nElements] : new ::AnalyzeVertex[nElements];
   }
   // Wrapper around operator delete
   static void delete_AnalyzeVertex(void *p) {
      delete ((::AnalyzeVertex*)p);
   }
   static void deleteArray_AnalyzeVertex(void *p) {
      delete [] ((::AnalyzeVertex*)p);
   }
   static void destruct_AnalyzeVertex(void *p) {
      typedef ::AnalyzeVertex current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AnalyzeVertex

namespace ROOT {
   static TClass *vectorlEvectorlEfloatgRsPgR_Dictionary();
   static void vectorlEvectorlEfloatgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEfloatgRsPgR(void *p = 0);
   static void *newArray_vectorlEvectorlEfloatgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEfloatgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEfloatgRsPgR(void *p);
   static void destruct_vectorlEvectorlEfloatgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<float> >*)
   {
      vector<vector<float> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<float> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<float> >", -2, "vector", 457,
                  typeid(vector<vector<float> >), DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEfloatgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<float> >) );
      instance.SetNew(&new_vectorlEvectorlEfloatgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEfloatgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEfloatgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEfloatgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEfloatgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<float> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<vector<float> >*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEfloatgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<float> >*)0x0)->GetClass();
      vectorlEvectorlEfloatgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEfloatgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEfloatgRsPgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<vector<float> > : new vector<vector<float> >;
   }
   static void *newArray_vectorlEvectorlEfloatgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<vector<float> >[nElements] : new vector<vector<float> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEfloatgRsPgR(void *p) {
      delete ((vector<vector<float> >*)p);
   }
   static void deleteArray_vectorlEvectorlEfloatgRsPgR(void *p) {
      delete [] ((vector<vector<float> >*)p);
   }
   static void destruct_vectorlEvectorlEfloatgRsPgR(void *p) {
      typedef vector<vector<float> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<float> >

namespace ROOT {
   static TClass *vectorlEmyVertexgR_Dictionary();
   static void vectorlEmyVertexgR_TClassManip(TClass*);
   static void *new_vectorlEmyVertexgR(void *p = 0);
   static void *newArray_vectorlEmyVertexgR(Long_t size, void *p);
   static void delete_vectorlEmyVertexgR(void *p);
   static void deleteArray_vectorlEmyVertexgR(void *p);
   static void destruct_vectorlEmyVertexgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<myVertex>*)
   {
      vector<myVertex> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<myVertex>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<myVertex>", -2, "vector", 457,
                  typeid(vector<myVertex>), DefineBehavior(ptr, ptr),
                  &vectorlEmyVertexgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<myVertex>) );
      instance.SetNew(&new_vectorlEmyVertexgR);
      instance.SetNewArray(&newArray_vectorlEmyVertexgR);
      instance.SetDelete(&delete_vectorlEmyVertexgR);
      instance.SetDeleteArray(&deleteArray_vectorlEmyVertexgR);
      instance.SetDestructor(&destruct_vectorlEmyVertexgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<myVertex> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<myVertex>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEmyVertexgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<myVertex>*)0x0)->GetClass();
      vectorlEmyVertexgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEmyVertexgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEmyVertexgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<myVertex> : new vector<myVertex>;
   }
   static void *newArray_vectorlEmyVertexgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<myVertex>[nElements] : new vector<myVertex>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEmyVertexgR(void *p) {
      delete ((vector<myVertex>*)p);
   }
   static void deleteArray_vectorlEmyVertexgR(void *p) {
      delete [] ((vector<myVertex>*)p);
   }
   static void destruct_vectorlEmyVertexgR(void *p) {
      typedef vector<myVertex> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<myVertex>

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 457,
                  typeid(vector<int>), DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlEhitgR_Dictionary();
   static void vectorlEhitgR_TClassManip(TClass*);
   static void *new_vectorlEhitgR(void *p = 0);
   static void *newArray_vectorlEhitgR(Long_t size, void *p);
   static void delete_vectorlEhitgR(void *p);
   static void deleteArray_vectorlEhitgR(void *p);
   static void destruct_vectorlEhitgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<hit>*)
   {
      vector<hit> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<hit>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<hit>", -2, "vector", 457,
                  typeid(vector<hit>), DefineBehavior(ptr, ptr),
                  &vectorlEhitgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<hit>) );
      instance.SetNew(&new_vectorlEhitgR);
      instance.SetNewArray(&newArray_vectorlEhitgR);
      instance.SetDelete(&delete_vectorlEhitgR);
      instance.SetDeleteArray(&deleteArray_vectorlEhitgR);
      instance.SetDestructor(&destruct_vectorlEhitgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<hit> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<hit>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEhitgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<hit>*)0x0)->GetClass();
      vectorlEhitgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEhitgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEhitgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<hit> : new vector<hit>;
   }
   static void *newArray_vectorlEhitgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<hit>[nElements] : new vector<hit>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEhitgR(void *p) {
      delete ((vector<hit>*)p);
   }
   static void deleteArray_vectorlEhitgR(void *p) {
      delete [] ((vector<hit>*)p);
   }
   static void destruct_vectorlEhitgR(void *p) {
      typedef vector<hit> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<hit>

namespace ROOT {
   static TClass *vectorlEfloatgR_Dictionary();
   static void vectorlEfloatgR_TClassManip(TClass*);
   static void *new_vectorlEfloatgR(void *p = 0);
   static void *newArray_vectorlEfloatgR(Long_t size, void *p);
   static void delete_vectorlEfloatgR(void *p);
   static void deleteArray_vectorlEfloatgR(void *p);
   static void destruct_vectorlEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<float>*)
   {
      vector<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<float>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<float>", -2, "vector", 457,
                  typeid(vector<float>), DefineBehavior(ptr, ptr),
                  &vectorlEfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<float>) );
      instance.SetNew(&new_vectorlEfloatgR);
      instance.SetNewArray(&newArray_vectorlEfloatgR);
      instance.SetDelete(&delete_vectorlEfloatgR);
      instance.SetDeleteArray(&deleteArray_vectorlEfloatgR);
      instance.SetDestructor(&destruct_vectorlEfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<float> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<float>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<float>*)0x0)->GetClass();
      vectorlEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEfloatgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<float> : new vector<float>;
   }
   static void *newArray_vectorlEfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<float>[nElements] : new vector<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEfloatgR(void *p) {
      delete ((vector<float>*)p);
   }
   static void deleteArray_vectorlEfloatgR(void *p) {
      delete [] ((vector<float>*)p);
   }
   static void destruct_vectorlEfloatgR(void *p) {
      typedef vector<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<float>

namespace ROOT {
   static TClass *vectorlETLorentzVectorgR_Dictionary();
   static void vectorlETLorentzVectorgR_TClassManip(TClass*);
   static void *new_vectorlETLorentzVectorgR(void *p = 0);
   static void *newArray_vectorlETLorentzVectorgR(Long_t size, void *p);
   static void delete_vectorlETLorentzVectorgR(void *p);
   static void deleteArray_vectorlETLorentzVectorgR(void *p);
   static void destruct_vectorlETLorentzVectorgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TLorentzVector>*)
   {
      vector<TLorentzVector> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TLorentzVector>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TLorentzVector>", -2, "vector", 457,
                  typeid(vector<TLorentzVector>), DefineBehavior(ptr, ptr),
                  &vectorlETLorentzVectorgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TLorentzVector>) );
      instance.SetNew(&new_vectorlETLorentzVectorgR);
      instance.SetNewArray(&newArray_vectorlETLorentzVectorgR);
      instance.SetDelete(&delete_vectorlETLorentzVectorgR);
      instance.SetDeleteArray(&deleteArray_vectorlETLorentzVectorgR);
      instance.SetDestructor(&destruct_vectorlETLorentzVectorgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TLorentzVector> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<TLorentzVector>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETLorentzVectorgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TLorentzVector>*)0x0)->GetClass();
      vectorlETLorentzVectorgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETLorentzVectorgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETLorentzVectorgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<TLorentzVector> : new vector<TLorentzVector>;
   }
   static void *newArray_vectorlETLorentzVectorgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<TLorentzVector>[nElements] : new vector<TLorentzVector>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETLorentzVectorgR(void *p) {
      delete ((vector<TLorentzVector>*)p);
   }
   static void deleteArray_vectorlETLorentzVectorgR(void *p) {
      delete [] ((vector<TLorentzVector>*)p);
   }
   static void destruct_vectorlETLorentzVectorgR(void *p) {
      typedef vector<TLorentzVector> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TLorentzVector>

namespace ROOT {
   static TClass *vectorlEPandoraNuTrackgR_Dictionary();
   static void vectorlEPandoraNuTrackgR_TClassManip(TClass*);
   static void *new_vectorlEPandoraNuTrackgR(void *p = 0);
   static void *newArray_vectorlEPandoraNuTrackgR(Long_t size, void *p);
   static void delete_vectorlEPandoraNuTrackgR(void *p);
   static void deleteArray_vectorlEPandoraNuTrackgR(void *p);
   static void destruct_vectorlEPandoraNuTrackgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<PandoraNuTrack>*)
   {
      vector<PandoraNuTrack> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<PandoraNuTrack>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<PandoraNuTrack>", -2, "vector", 457,
                  typeid(vector<PandoraNuTrack>), DefineBehavior(ptr, ptr),
                  &vectorlEPandoraNuTrackgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<PandoraNuTrack>) );
      instance.SetNew(&new_vectorlEPandoraNuTrackgR);
      instance.SetNewArray(&newArray_vectorlEPandoraNuTrackgR);
      instance.SetDelete(&delete_vectorlEPandoraNuTrackgR);
      instance.SetDeleteArray(&deleteArray_vectorlEPandoraNuTrackgR);
      instance.SetDestructor(&destruct_vectorlEPandoraNuTrackgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<PandoraNuTrack> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<PandoraNuTrack>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEPandoraNuTrackgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<PandoraNuTrack>*)0x0)->GetClass();
      vectorlEPandoraNuTrackgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEPandoraNuTrackgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEPandoraNuTrackgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<PandoraNuTrack> : new vector<PandoraNuTrack>;
   }
   static void *newArray_vectorlEPandoraNuTrackgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<PandoraNuTrack>[nElements] : new vector<PandoraNuTrack>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEPandoraNuTrackgR(void *p) {
      delete ((vector<PandoraNuTrack>*)p);
   }
   static void deleteArray_vectorlEPandoraNuTrackgR(void *p) {
      delete [] ((vector<PandoraNuTrack>*)p);
   }
   static void destruct_vectorlEPandoraNuTrackgR(void *p) {
      typedef vector<PandoraNuTrack> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<PandoraNuTrack>

namespace ROOT {
   static TClass *vectorlEGENIEinteractiongR_Dictionary();
   static void vectorlEGENIEinteractiongR_TClassManip(TClass*);
   static void *new_vectorlEGENIEinteractiongR(void *p = 0);
   static void *newArray_vectorlEGENIEinteractiongR(Long_t size, void *p);
   static void delete_vectorlEGENIEinteractiongR(void *p);
   static void deleteArray_vectorlEGENIEinteractiongR(void *p);
   static void destruct_vectorlEGENIEinteractiongR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<GENIEinteraction>*)
   {
      vector<GENIEinteraction> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<GENIEinteraction>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<GENIEinteraction>", -2, "vector", 457,
                  typeid(vector<GENIEinteraction>), DefineBehavior(ptr, ptr),
                  &vectorlEGENIEinteractiongR_Dictionary, isa_proxy, 0,
                  sizeof(vector<GENIEinteraction>) );
      instance.SetNew(&new_vectorlEGENIEinteractiongR);
      instance.SetNewArray(&newArray_vectorlEGENIEinteractiongR);
      instance.SetDelete(&delete_vectorlEGENIEinteractiongR);
      instance.SetDeleteArray(&deleteArray_vectorlEGENIEinteractiongR);
      instance.SetDestructor(&destruct_vectorlEGENIEinteractiongR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<GENIEinteraction> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<GENIEinteraction>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEGENIEinteractiongR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<GENIEinteraction>*)0x0)->GetClass();
      vectorlEGENIEinteractiongR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEGENIEinteractiongR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEGENIEinteractiongR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<GENIEinteraction> : new vector<GENIEinteraction>;
   }
   static void *newArray_vectorlEGENIEinteractiongR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<GENIEinteraction>[nElements] : new vector<GENIEinteraction>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEGENIEinteractiongR(void *p) {
      delete ((vector<GENIEinteraction>*)p);
   }
   static void deleteArray_vectorlEGENIEinteractiongR(void *p) {
      delete [] ((vector<GENIEinteraction>*)p);
   }
   static void destruct_vectorlEGENIEinteractiongR(void *p) {
      typedef vector<GENIEinteraction> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<GENIEinteraction>

namespace {
  void TriggerDictionaryInitialization_libAnalysisTreesInformation_CCQEPackage_Impl() {
    static const char* headers[] = {
"AnalyzeVertex.h",
"calcEventTopologies.h",
"myVertex.h",
0
    };
    static const char* includePaths[] = {
"/Users/erezcohen/larlite/UserDev/mySoftware",
"/Users/erezcohen/larlite/UserDev/MyLarLite/MyPackage",
"/Users/erezcohen/larlite/UserDev/LarLite/MyPackage",
"/Users/erezcohen/larlite/UserDev/BasicTool/GeoAlgo",
"/Users/erezcohen/larlite/UserDev/AnalysisTreesInformation/AnaTreesPackage",
"/Users/erezcohen/larlite/core",
"/Users/erezcohen/root6/root-6.04.10/include",
"/Users/erezcohen/larlite/UserDev/AnalysisTreesInformation/CCQEPackage/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$calcEventTopologies.h")))  calcEventTopologies;
class __attribute__((annotate("$clingAutoload$AnalyzeVertex.h")))  myVertex;
class __attribute__((annotate("$clingAutoload$AnalyzeVertex.h")))  AnalyzeVertex;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "AnalyzeVertex.h"
#include "calcEventTopologies.h"
#include "myVertex.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"AnalyzeVertex", payloadCode, "@",
"calcEventTopologies", payloadCode, "@",
"myVertex", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libAnalysisTreesInformation_CCQEPackage",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libAnalysisTreesInformation_CCQEPackage_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libAnalysisTreesInformation_CCQEPackage_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libAnalysisTreesInformation_CCQEPackage() {
  TriggerDictionaryInitialization_libAnalysisTreesInformation_CCQEPackage_Impl();
}
