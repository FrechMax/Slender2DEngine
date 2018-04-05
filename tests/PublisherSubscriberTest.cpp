#define BOOST_TEST_MODULE START_MODULE
#include "boost/test/unit_test.hpp"
#include <string>
#include <iostream>

#include "Core\Event\PostSubscribeEvent.h"

struct ExampleEvent 
{
	ExampleEvent(int i, float f) 
		: i(i), f(f)
	{} 
	
	int i;
	float f;
};


struct ExampleSubscriber : public S2D::Event::Subscriber<ExampleSubscriber>
{
public:
	int i = 0;
	float f = 0.0;

	void handle(ExampleEvent& e)
	{
		f = e.f;
		i = e.i;
	}
};

struct Greeting
{
	std::string greeting;
};


BOOST_AUTO_TEST_SUITE(TestSuite_PublisherSubscriber)

BOOST_AUTO_TEST_CASE(PublisherInvokeArgsOnArgs) 
{
	ExampleSubscriber exSub;
	exSub.subscribeFor<ExampleEvent>();
	
	S2D::Event::Publisher<ExampleEvent>::invoke(3, 2.0f);

	BOOST_TEST(exSub.i == 3);
	BOOST_TEST(exSub.f == 2.0f);
}

BOOST_AUTO_TEST_CASE(PublisherInvokeOnEvent)
{
	ExampleSubscriber exSub;
	exSub.subscribeFor<ExampleEvent>();
	ExampleEvent exEvent(5, 2.3f);

	S2D::Event::Publisher<ExampleEvent>::invoke(exEvent);

	BOOST_TEST(exSub.i == 5);
	BOOST_TEST(exSub.f == 2.3f);
}

BOOST_AUTO_TEST_CASE(SubscriberMustUnregisterManually)
{
	ExampleSubscriber* exSub = new ExampleSubscriber();
	exSub->subscribeFor<ExampleEvent>();
	ExampleEvent exEvent(5, 2.3f);

	BOOST_TEST(S2D::Event::Publisher<ExampleEvent>::getSubscriberCount() == 1);

	delete exSub;
	
	S2D::Event::Publisher<ExampleEvent>::invoke(exEvent);

	BOOST_TEST(S2D::Event::Publisher<ExampleEvent>::getSubscriberCount() == 1);
}


BOOST_AUTO_TEST_SUITE_END()