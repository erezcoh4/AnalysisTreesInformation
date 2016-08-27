// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME AnalysisTreesInformation_AnaTreesPackageDict

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
#include "GENIEinteraction.h"
#include "PandoraNuTrack.h"
#include "cumputeAnaTree.h"
#include "mutual_vertex.h"
#include "nuInteraction.h"
#include "track_vertex.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *box_Dictionary();
   static void box_TClassManip(TClass*);
   static void *new_box(void *p = 0);
   static void *newArray_box(Long_t size, void *p);
   static void delete_box(void *p);
   static void deleteArray_box(void *p);
   static void destruct_box(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::box*)
   {
      ::box *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::box));
      static ::ROOT::TGenericClassInfo 
         instance("box", "PandoraNuTrack.h", 30,
                  typeid(::box), DefineBehavior(ptr, ptr),
                  &box_Dictionary, isa_proxy, 4,
                  sizeof(::box) );
      instance.SetNew(&new_box);
      instance.SetNewArray(&newArray_box);
      instance.SetDelete(&delete_box);
      instance.SetDeleteArray(&deleteArray_box);
      instance.SetDestructor(&destruct_box);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::box*)
   {
      return GenerateInitInstanceLocal((::box*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::box*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *box_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::box*)0x0)->GetClass();
      box_TClassManip(theClass);
   return theClass;
   }

   static void box_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *PandoraNuTrack_Dictionary();
   static void PandoraNuTrack_TClassManip(TClass*);
   static void *new_PandoraNuTrack(void *p = 0);
   static void *newArray_PandoraNuTrack(Long_t size, void *p);
   static void delete_PandoraNuTrack(void *p);
   static void deleteArray_PandoraNuTrack(void *p);
   static void destruct_PandoraNuTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PandoraNuTrack*)
   {
      ::PandoraNuTrack *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PandoraNuTrack));
      static ::ROOT::TGenericClassInfo 
         instance("PandoraNuTrack", "PandoraNuTrack.h", 67,
                  typeid(::PandoraNuTrack), DefineBehavior(ptr, ptr),
                  &PandoraNuTrack_Dictionary, isa_proxy, 4,
                  sizeof(::PandoraNuTrack) );
      instance.SetNew(&new_PandoraNuTrack);
      instance.SetNewArray(&newArray_PandoraNuTrack);
      instance.SetDelete(&delete_PandoraNuTrack);
      instance.SetDeleteArray(&deleteArray_PandoraNuTrack);
      instance.SetDestructor(&destruct_PandoraNuTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PandoraNuTrack*)
   {
      return GenerateInitInstanceLocal((::PandoraNuTrack*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::PandoraNuTrack*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PandoraNuTrack_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::PandoraNuTrack*)0x0)->GetClass();
      PandoraNuTrack_TClassManip(theClass);
   return theClass;
   }

   static void PandoraNuTrack_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *cumputeAnaTree_Dictionary();
   static void cumputeAnaTree_TClassManip(TClass*);
   static void *new_cumputeAnaTree(void *p = 0);
   static void *newArray_cumputeAnaTree(Long_t size, void *p);
   static void delete_cumputeAnaTree(void *p);
   static void deleteArray_cumputeAnaTree(void *p);
   static void destruct_cumputeAnaTree(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cumputeAnaTree*)
   {
      ::cumputeAnaTree *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::cumputeAnaTree));
      static ::ROOT::TGenericClassInfo 
         instance("cumputeAnaTree", "cumputeAnaTree.h", 37,
                  typeid(::cumputeAnaTree), DefineBehavior(ptr, ptr),
                  &cumputeAnaTree_Dictionary, isa_proxy, 4,
                  sizeof(::cumputeAnaTree) );
      instance.SetNew(&new_cumputeAnaTree);
      instance.SetNewArray(&newArray_cumputeAnaTree);
      instance.SetDelete(&delete_cumputeAnaTree);
      instance.SetDeleteArray(&deleteArray_cumputeAnaTree);
      instance.SetDestructor(&destruct_cumputeAnaTree);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cumputeAnaTree*)
   {
      return GenerateInitInstanceLocal((::cumputeAnaTree*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::cumputeAnaTree*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *cumputeAnaTree_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::cumputeAnaTree*)0x0)->GetClass();
      cumputeAnaTree_TClassManip(theClass);
   return theClass;
   }

   static void cumputeAnaTree_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *nuInteraction_Dictionary();
   static void nuInteraction_TClassManip(TClass*);
   static void *new_nuInteraction(void *p = 0);
   static void *newArray_nuInteraction(Long_t size, void *p);
   static void delete_nuInteraction(void *p);
   static void deleteArray_nuInteraction(void *p);
   static void destruct_nuInteraction(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::nuInteraction*)
   {
      ::nuInteraction *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::nuInteraction));
      static ::ROOT::TGenericClassInfo 
         instance("nuInteraction", "nuInteraction.h", 32,
                  typeid(::nuInteraction), DefineBehavior(ptr, ptr),
                  &nuInteraction_Dictionary, isa_proxy, 4,
                  sizeof(::nuInteraction) );
      instance.SetNew(&new_nuInteraction);
      instance.SetNewArray(&newArray_nuInteraction);
      instance.SetDelete(&delete_nuInteraction);
      instance.SetDeleteArray(&deleteArray_nuInteraction);
      instance.SetDestructor(&destruct_nuInteraction);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::nuInteraction*)
   {
      return GenerateInitInstanceLocal((::nuInteraction*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::nuInteraction*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *nuInteraction_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::nuInteraction*)0x0)->GetClass();
      nuInteraction_TClassManip(theClass);
   return theClass;
   }

   static void nuInteraction_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *GENIEinteraction_Dictionary();
   static void GENIEinteraction_TClassManip(TClass*);
   static void *new_GENIEinteraction(void *p = 0);
   static void *newArray_GENIEinteraction(Long_t size, void *p);
   static void delete_GENIEinteraction(void *p);
   static void deleteArray_GENIEinteraction(void *p);
   static void destruct_GENIEinteraction(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::GENIEinteraction*)
   {
      ::GENIEinteraction *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::GENIEinteraction));
      static ::ROOT::TGenericClassInfo 
         instance("GENIEinteraction", "GENIEinteraction.h", 32,
                  typeid(::GENIEinteraction), DefineBehavior(ptr, ptr),
                  &GENIEinteraction_Dictionary, isa_proxy, 4,
                  sizeof(::GENIEinteraction) );
      instance.SetNew(&new_GENIEinteraction);
      instance.SetNewArray(&newArray_GENIEinteraction);
      instance.SetDelete(&delete_GENIEinteraction);
      instance.SetDeleteArray(&deleteArray_GENIEinteraction);
      instance.SetDestructor(&destruct_GENIEinteraction);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::GENIEinteraction*)
   {
      return GenerateInitInstanceLocal((::GENIEinteraction*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::GENIEinteraction*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *GENIEinteraction_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::GENIEinteraction*)0x0)->GetClass();
      GENIEinteraction_TClassManip(theClass);
   return theClass;
   }

   static void GENIEinteraction_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *track_vertex_Dictionary();
   static void track_vertex_TClassManip(TClass*);
   static void *new_track_vertex(void *p = 0);
   static void *newArray_track_vertex(Long_t size, void *p);
   static void delete_track_vertex(void *p);
   static void deleteArray_track_vertex(void *p);
   static void destruct_track_vertex(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::track_vertex*)
   {
      ::track_vertex *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::track_vertex));
      static ::ROOT::TGenericClassInfo 
         instance("track_vertex", "track_vertex.h", 26,
                  typeid(::track_vertex), DefineBehavior(ptr, ptr),
                  &track_vertex_Dictionary, isa_proxy, 4,
                  sizeof(::track_vertex) );
      instance.SetNew(&new_track_vertex);
      instance.SetNewArray(&newArray_track_vertex);
      instance.SetDelete(&delete_track_vertex);
      instance.SetDeleteArray(&deleteArray_track_vertex);
      instance.SetDestructor(&destruct_track_vertex);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::track_vertex*)
   {
      return GenerateInitInstanceLocal((::track_vertex*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::track_vertex*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *track_vertex_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::track_vertex*)0x0)->GetClass();
      track_vertex_TClassManip(theClass);
   return theClass;
   }

   static void track_vertex_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *mutual_vertex_Dictionary();
   static void mutual_vertex_TClassManip(TClass*);
   static void *new_mutual_vertex(void *p = 0);
   static void *newArray_mutual_vertex(Long_t size, void *p);
   static void delete_mutual_vertex(void *p);
   static void deleteArray_mutual_vertex(void *p);
   static void destruct_mutual_vertex(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::mutual_vertex*)
   {
      ::mutual_vertex *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::mutual_vertex));
      static ::ROOT::TGenericClassInfo 
         instance("mutual_vertex", "mutual_vertex.h", 26,
                  typeid(::mutual_vertex), DefineBehavior(ptr, ptr),
                  &mutual_vertex_Dictionary, isa_proxy, 4,
                  sizeof(::mutual_vertex) );
      instance.SetNew(&new_mutual_vertex);
      instance.SetNewArray(&newArray_mutual_vertex);
      instance.SetDelete(&delete_mutual_vertex);
      instance.SetDeleteArray(&deleteArray_mutual_vertex);
      instance.SetDestructor(&destruct_mutual_vertex);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::mutual_vertex*)
   {
      return GenerateInitInstanceLocal((::mutual_vertex*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::mutual_vertex*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *mutual_vertex_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::mutual_vertex*)0x0)->GetClass();
      mutual_vertex_TClassManip(theClass);
   return theClass;
   }

   static void mutual_vertex_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_box(void *p) {
      return  p ? new(p) ::box : new ::box;
   }
   static void *newArray_box(Long_t nElements, void *p) {
      return p ? new(p) ::box[nElements] : new ::box[nElements];
   }
   // Wrapper around operator delete
   static void delete_box(void *p) {
      delete ((::box*)p);
   }
   static void deleteArray_box(void *p) {
      delete [] ((::box*)p);
   }
   static void destruct_box(void *p) {
      typedef ::box current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::box

namespace ROOT {
   // Wrappers around operator new
   static void *new_PandoraNuTrack(void *p) {
      return  p ? new(p) ::PandoraNuTrack : new ::PandoraNuTrack;
   }
   static void *newArray_PandoraNuTrack(Long_t nElements, void *p) {
      return p ? new(p) ::PandoraNuTrack[nElements] : new ::PandoraNuTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_PandoraNuTrack(void *p) {
      delete ((::PandoraNuTrack*)p);
   }
   static void deleteArray_PandoraNuTrack(void *p) {
      delete [] ((::PandoraNuTrack*)p);
   }
   static void destruct_PandoraNuTrack(void *p) {
      typedef ::PandoraNuTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PandoraNuTrack

namespace ROOT {
   // Wrappers around operator new
   static void *new_cumputeAnaTree(void *p) {
      return  p ? new(p) ::cumputeAnaTree : new ::cumputeAnaTree;
   }
   static void *newArray_cumputeAnaTree(Long_t nElements, void *p) {
      return p ? new(p) ::cumputeAnaTree[nElements] : new ::cumputeAnaTree[nElements];
   }
   // Wrapper around operator delete
   static void delete_cumputeAnaTree(void *p) {
      delete ((::cumputeAnaTree*)p);
   }
   static void deleteArray_cumputeAnaTree(void *p) {
      delete [] ((::cumputeAnaTree*)p);
   }
   static void destruct_cumputeAnaTree(void *p) {
      typedef ::cumputeAnaTree current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cumputeAnaTree

namespace ROOT {
   // Wrappers around operator new
   static void *new_nuInteraction(void *p) {
      return  p ? new(p) ::nuInteraction : new ::nuInteraction;
   }
   static void *newArray_nuInteraction(Long_t nElements, void *p) {
      return p ? new(p) ::nuInteraction[nElements] : new ::nuInteraction[nElements];
   }
   // Wrapper around operator delete
   static void delete_nuInteraction(void *p) {
      delete ((::nuInteraction*)p);
   }
   static void deleteArray_nuInteraction(void *p) {
      delete [] ((::nuInteraction*)p);
   }
   static void destruct_nuInteraction(void *p) {
      typedef ::nuInteraction current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::nuInteraction

namespace ROOT {
   // Wrappers around operator new
   static void *new_GENIEinteraction(void *p) {
      return  p ? new(p) ::GENIEinteraction : new ::GENIEinteraction;
   }
   static void *newArray_GENIEinteraction(Long_t nElements, void *p) {
      return p ? new(p) ::GENIEinteraction[nElements] : new ::GENIEinteraction[nElements];
   }
   // Wrapper around operator delete
   static void delete_GENIEinteraction(void *p) {
      delete ((::GENIEinteraction*)p);
   }
   static void deleteArray_GENIEinteraction(void *p) {
      delete [] ((::GENIEinteraction*)p);
   }
   static void destruct_GENIEinteraction(void *p) {
      typedef ::GENIEinteraction current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::GENIEinteraction

namespace ROOT {
   // Wrappers around operator new
   static void *new_track_vertex(void *p) {
      return  p ? new(p) ::track_vertex : new ::track_vertex;
   }
   static void *newArray_track_vertex(Long_t nElements, void *p) {
      return p ? new(p) ::track_vertex[nElements] : new ::track_vertex[nElements];
   }
   // Wrapper around operator delete
   static void delete_track_vertex(void *p) {
      delete ((::track_vertex*)p);
   }
   static void deleteArray_track_vertex(void *p) {
      delete [] ((::track_vertex*)p);
   }
   static void destruct_track_vertex(void *p) {
      typedef ::track_vertex current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::track_vertex

namespace ROOT {
   // Wrappers around operator new
   static void *new_mutual_vertex(void *p) {
      return  p ? new(p) ::mutual_vertex : new ::mutual_vertex;
   }
   static void *newArray_mutual_vertex(Long_t nElements, void *p) {
      return p ? new(p) ::mutual_vertex[nElements] : new ::mutual_vertex[nElements];
   }
   // Wrapper around operator delete
   static void delete_mutual_vertex(void *p) {
      delete ((::mutual_vertex*)p);
   }
   static void deleteArray_mutual_vertex(void *p) {
      delete [] ((::mutual_vertex*)p);
   }
   static void destruct_mutual_vertex(void *p) {
      typedef ::mutual_vertex current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::mutual_vertex

namespace ROOT {
   static TClass *vectorlEtrack_vertexgR_Dictionary();
   static void vectorlEtrack_vertexgR_TClassManip(TClass*);
   static void *new_vectorlEtrack_vertexgR(void *p = 0);
   static void *newArray_vectorlEtrack_vertexgR(Long_t size, void *p);
   static void delete_vectorlEtrack_vertexgR(void *p);
   static void deleteArray_vectorlEtrack_vertexgR(void *p);
   static void destruct_vectorlEtrack_vertexgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<track_vertex>*)
   {
      vector<track_vertex> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<track_vertex>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<track_vertex>", -2, "vector", 457,
                  typeid(vector<track_vertex>), DefineBehavior(ptr, ptr),
                  &vectorlEtrack_vertexgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<track_vertex>) );
      instance.SetNew(&new_vectorlEtrack_vertexgR);
      instance.SetNewArray(&newArray_vectorlEtrack_vertexgR);
      instance.SetDelete(&delete_vectorlEtrack_vertexgR);
      instance.SetDeleteArray(&deleteArray_vectorlEtrack_vertexgR);
      instance.SetDestructor(&destruct_vectorlEtrack_vertexgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<track_vertex> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<track_vertex>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEtrack_vertexgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<track_vertex>*)0x0)->GetClass();
      vectorlEtrack_vertexgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEtrack_vertexgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEtrack_vertexgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<track_vertex> : new vector<track_vertex>;
   }
   static void *newArray_vectorlEtrack_vertexgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<track_vertex>[nElements] : new vector<track_vertex>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEtrack_vertexgR(void *p) {
      delete ((vector<track_vertex>*)p);
   }
   static void deleteArray_vectorlEtrack_vertexgR(void *p) {
      delete [] ((vector<track_vertex>*)p);
   }
   static void destruct_vectorlEtrack_vertexgR(void *p) {
      typedef vector<track_vertex> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<track_vertex>

namespace ROOT {
   static TClass *vectorlEnuInteractiongR_Dictionary();
   static void vectorlEnuInteractiongR_TClassManip(TClass*);
   static void *new_vectorlEnuInteractiongR(void *p = 0);
   static void *newArray_vectorlEnuInteractiongR(Long_t size, void *p);
   static void delete_vectorlEnuInteractiongR(void *p);
   static void deleteArray_vectorlEnuInteractiongR(void *p);
   static void destruct_vectorlEnuInteractiongR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<nuInteraction>*)
   {
      vector<nuInteraction> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<nuInteraction>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<nuInteraction>", -2, "vector", 457,
                  typeid(vector<nuInteraction>), DefineBehavior(ptr, ptr),
                  &vectorlEnuInteractiongR_Dictionary, isa_proxy, 0,
                  sizeof(vector<nuInteraction>) );
      instance.SetNew(&new_vectorlEnuInteractiongR);
      instance.SetNewArray(&newArray_vectorlEnuInteractiongR);
      instance.SetDelete(&delete_vectorlEnuInteractiongR);
      instance.SetDeleteArray(&deleteArray_vectorlEnuInteractiongR);
      instance.SetDestructor(&destruct_vectorlEnuInteractiongR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<nuInteraction> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<nuInteraction>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEnuInteractiongR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<nuInteraction>*)0x0)->GetClass();
      vectorlEnuInteractiongR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEnuInteractiongR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEnuInteractiongR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<nuInteraction> : new vector<nuInteraction>;
   }
   static void *newArray_vectorlEnuInteractiongR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<nuInteraction>[nElements] : new vector<nuInteraction>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEnuInteractiongR(void *p) {
      delete ((vector<nuInteraction>*)p);
   }
   static void deleteArray_vectorlEnuInteractiongR(void *p) {
      delete [] ((vector<nuInteraction>*)p);
   }
   static void destruct_vectorlEnuInteractiongR(void *p) {
      typedef vector<nuInteraction> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<nuInteraction>

namespace ROOT {
   static TClass *vectorlEmutual_vertexgR_Dictionary();
   static void vectorlEmutual_vertexgR_TClassManip(TClass*);
   static void *new_vectorlEmutual_vertexgR(void *p = 0);
   static void *newArray_vectorlEmutual_vertexgR(Long_t size, void *p);
   static void delete_vectorlEmutual_vertexgR(void *p);
   static void deleteArray_vectorlEmutual_vertexgR(void *p);
   static void destruct_vectorlEmutual_vertexgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<mutual_vertex>*)
   {
      vector<mutual_vertex> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<mutual_vertex>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<mutual_vertex>", -2, "vector", 457,
                  typeid(vector<mutual_vertex>), DefineBehavior(ptr, ptr),
                  &vectorlEmutual_vertexgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<mutual_vertex>) );
      instance.SetNew(&new_vectorlEmutual_vertexgR);
      instance.SetNewArray(&newArray_vectorlEmutual_vertexgR);
      instance.SetDelete(&delete_vectorlEmutual_vertexgR);
      instance.SetDeleteArray(&deleteArray_vectorlEmutual_vertexgR);
      instance.SetDestructor(&destruct_vectorlEmutual_vertexgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<mutual_vertex> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<mutual_vertex>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEmutual_vertexgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<mutual_vertex>*)0x0)->GetClass();
      vectorlEmutual_vertexgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEmutual_vertexgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEmutual_vertexgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<mutual_vertex> : new vector<mutual_vertex>;
   }
   static void *newArray_vectorlEmutual_vertexgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<mutual_vertex>[nElements] : new vector<mutual_vertex>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEmutual_vertexgR(void *p) {
      delete ((vector<mutual_vertex>*)p);
   }
   static void deleteArray_vectorlEmutual_vertexgR(void *p) {
      delete [] ((vector<mutual_vertex>*)p);
   }
   static void destruct_vectorlEmutual_vertexgR(void *p) {
      typedef vector<mutual_vertex> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<mutual_vertex>

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
   static TClass *vectorlETVector3gR_Dictionary();
   static void vectorlETVector3gR_TClassManip(TClass*);
   static void *new_vectorlETVector3gR(void *p = 0);
   static void *newArray_vectorlETVector3gR(Long_t size, void *p);
   static void delete_vectorlETVector3gR(void *p);
   static void deleteArray_vectorlETVector3gR(void *p);
   static void destruct_vectorlETVector3gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TVector3>*)
   {
      vector<TVector3> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TVector3>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TVector3>", -2, "vector", 457,
                  typeid(vector<TVector3>), DefineBehavior(ptr, ptr),
                  &vectorlETVector3gR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TVector3>) );
      instance.SetNew(&new_vectorlETVector3gR);
      instance.SetNewArray(&newArray_vectorlETVector3gR);
      instance.SetDelete(&delete_vectorlETVector3gR);
      instance.SetDeleteArray(&deleteArray_vectorlETVector3gR);
      instance.SetDestructor(&destruct_vectorlETVector3gR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TVector3> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<TVector3>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETVector3gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TVector3>*)0x0)->GetClass();
      vectorlETVector3gR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETVector3gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETVector3gR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<TVector3> : new vector<TVector3>;
   }
   static void *newArray_vectorlETVector3gR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<TVector3>[nElements] : new vector<TVector3>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETVector3gR(void *p) {
      delete ((vector<TVector3>*)p);
   }
   static void deleteArray_vectorlETVector3gR(void *p) {
      delete [] ((vector<TVector3>*)p);
   }
   static void destruct_vectorlETVector3gR(void *p) {
      typedef vector<TVector3> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TVector3>

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
  void TriggerDictionaryInitialization_libAnalysisTreesInformation_AnaTreesPackage_Impl() {
    static const char* headers[] = {
"GENIEinteraction.h",
"PandoraNuTrack.h",
"cumputeAnaTree.h",
"mutual_vertex.h",
"nuInteraction.h",
"track_vertex.h",
0
    };
    static const char* includePaths[] = {
"/Users/erezcohen/larlite/UserDev/mySoftware",
"/Users/erezcohen/larlite/UserDev/MyLarLite/MyPackage",
"/Users/erezcohen/larlite/UserDev/BasicTool/GeoAlgo",
"/Users/erezcohen/larlite/core",
"/Users/erezcohen/root6/root-6.04.10/include",
"/Users/erezcohen/larlite/UserDev/AnalysisTreesInformation/AnaTreesPackage/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
struct __attribute__((annotate("$clingAutoload$PandoraNuTrack.h")))  box;
class __attribute__((annotate("$clingAutoload$PandoraNuTrack.h")))  PandoraNuTrack;
class __attribute__((annotate("$clingAutoload$cumputeAnaTree.h")))  cumputeAnaTree;
class __attribute__((annotate("$clingAutoload$cumputeAnaTree.h")))  nuInteraction;
class __attribute__((annotate("$clingAutoload$GENIEinteraction.h")))  GENIEinteraction;
class __attribute__((annotate("$clingAutoload$cumputeAnaTree.h")))  track_vertex;
class __attribute__((annotate("$clingAutoload$cumputeAnaTree.h")))  mutual_vertex;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "GENIEinteraction.h"
#include "PandoraNuTrack.h"
#include "cumputeAnaTree.h"
#include "mutual_vertex.h"
#include "nuInteraction.h"
#include "track_vertex.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"GENIEinteraction", payloadCode, "@",
"PandoraNuTrack", payloadCode, "@",
"box", payloadCode, "@",
"cumputeAnaTree", payloadCode, "@",
"mutual_vertex", payloadCode, "@",
"nuInteraction", payloadCode, "@",
"track_vertex", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libAnalysisTreesInformation_AnaTreesPackage",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libAnalysisTreesInformation_AnaTreesPackage_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libAnalysisTreesInformation_AnaTreesPackage_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libAnalysisTreesInformation_AnaTreesPackage() {
  TriggerDictionaryInitialization_libAnalysisTreesInformation_AnaTreesPackage_Impl();
}
