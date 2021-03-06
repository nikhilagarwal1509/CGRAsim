#include "cgra_network.h"
#include "cgra_pe.h"
#include "cgra.h"

namespace platy {
namespace sim {
namespace cgra {

#if 0
/* nzb: This is a magic network. Rename. */
void BusNetwork::sendToken(PeIdx peid, TokenStore::Token tok) {
    ProcessingElement * pe = cgra->getProcessingElement(peid);
    if (!pe->acceptToken(tok)) {
        qassert(false);
        Cycles newEventTime = cgra->now() + cgra->networkDelay + cgra->setTokenFailDelay;
        CgraEvent* event =  new SendTokenCgraEvent(newEventTime, peid, tok);
        cgra->pushEvent(event);
    }
}
#endif

void BusNetwork::sendToken(NetworkPort* src, const std::vector<Location>& dsts, Word value, CbIdx cbidx) {
    Cycles timestamp = bandwidthPort.grab(delay);
    CgraEvent* event = new BusEvent{cgra, this, src, dsts, value, cbidx};
    cgra->pushEvent(event, timestamp);
}

void BusNetwork::BusEvent::go() {
    for (size_t i = 0; i < dsts.size(); i++) {
        auto dst = dsts.front();
        TokenStore::Token tok{dst.pos, value, dst.inst, cbidx};
        auto* dstPe = cgra->getProcessingElement(dst.pe);
        dsts.pop_front();
        if (!dstPe->acceptToken(tok)) {
            dsts.push_back(dst);
        }
    }

    bool retry = !dsts.empty();
    if (retry) {
        Cycles timestamp = network->bandwidthPort.grab(network->delay, 1_cycles);
        cgra->pushEvent(this, timestamp);
    } else {
        src->acknowledgeToken();
    }
}

}
}
}
