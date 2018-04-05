#include "Core\Event\PostSubscribeEvent.h"

using namespace S2D::Event;

template <typename TEvent>
typename Publisher<TEvent>::EventHandlerList Publisher<TEvent>::mEventHandlerList;