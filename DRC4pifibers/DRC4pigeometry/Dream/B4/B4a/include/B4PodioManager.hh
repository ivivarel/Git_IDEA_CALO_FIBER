#ifndef B4PODIOMANAGER_H
#define B4PODIOMANAGER_H

#include <map>

#include "podio/EventStore.h"
#include "podio/ROOTWriter.h"

#include "globals.hh"

#include "G4AutoLock.hh"

namespace { G4Mutex B4PodioActionMutex = G4MUTEX_INITIALIZER; }

class B4PodioManager
{
 public:
  static B4PodioManager * Instance();
  void SetFilePrefix (G4String filename){m_filename_prefix = filename;}
  void SetFileSuffix(G4String filename){m_filename_suffix = filename;}
  void FinishEvent();
  bool Finish();
  podio::EventStore * GetEvtStore();
  podio::ROOTWriter * GetWriter();
  template<class T> T * Register(const G4String collectionname);
  
protected:
  G4String m_filename_prefix;
  G4String m_filename_suffix; 
  std::map<int,podio::EventStore *> m_map_store;
  std::map<int,podio::ROOTWriter *> m_map_writer;  
private:
  static B4PodioManager * m_inst_;
  B4PodioManager();
  
  
};

template <class T> T * B4PodioManager::Register(const G4String collectionname)
{
  B4PodioManager* podioManager = B4PodioManager::Instance();
  podio::EventStore * l_evtstore = podioManager->GetEvtStore();
  if (l_evtstore == NULL) return NULL;
  podio::ROOTWriter * l_writer = podioManager->GetWriter();
  
  G4AutoLock lock(&B4PodioActionMutex);
  
  T * retval = new T();
  std::cout << "In register " << typeid(T).name() << std::endl;
  l_evtstore->registerCollection(collectionname,retval);
  l_writer->registerForWrite(collectionname);
  
  return retval;
}

#endif 
