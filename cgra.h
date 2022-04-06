
#pragma once

// #include "../engine.h"
#include "cgra_defs.h"
#include "cgra_tok_st.h"
#include "cgra_event.h"
#include "cgra_input_port.h"

namespace platy {
namespace sim {
namespace cgra {

class Network;
class ProcessingElement;
/**
 * @brief A single CGRA fabric.
 */
class Cgra {//: public Engine {
public:
    Cgra(
        // const std::string& name,
        // TileIdx tile,
        // EngineIdx _engIdx,
        uint32_t _numPes,
        uint32_t _numInsts,
        uint32_t _numThrds);

    // void setCaches(BaseCache* _l1i, BaseCache* _l1d) : l1i(_l1i), l1d(_l1d) {;}
    // BaseCache* getL1d() { return l1d; }
    // void configure(const FunctionConfiguration& functionConf);
    void configure(const FunctionConfiguration& functionConf);
    // void unconfigure(void* funcPtr) {}
    // void execute(std::shared_ptr<TaskReq> req);

    void execute(std::shared_ptr<TaskReq> req);
    
    void pushEvent(CgraEvent* event, Cycles timestamp);

    class OutOfEvents {};
    
    void tick();

    inline Cycles now() const { return currentTime; }

    ProcessingElement* getProcessingElement(PeIdx peid) {
        return processingElements.at(peid);
    }
    const ProcessingElement* getProcessingElement(PeIdx peid) const {
        return const_cast<Cgra*>(this)->getProcessingElement(peid);     
    }

    Network* getNetwork();

private:
    Cycles currentTime;

    // BaseCache* l1i;
    // BaseCache* l1d;
    std::priority_queue<std::pair<Cycles, CgraEvent*>, 
        std::vector<std::pair<Cycles, CgraEvent*>>, Cmprtr> eventQueue;
    CbIdx cbidx;
    Network* network;
    StrongVec<PeIdx, ProcessingElement*> processingElements;
    std::vector<std::vector<Location>> inputDestinationMap;
    InputPort* inputPort;

    void loadBitstream(Config& bitstream);
    void loadStaticParams(Config& bitstream, void* context);
    void loadInputMap(Config& bitstream);
    void loadRuntimeInputs(Word* inputs);

};

}  // namespace cgra
}  // namespace sim
}  // namespace platy
