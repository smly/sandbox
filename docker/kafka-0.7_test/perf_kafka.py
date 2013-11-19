from kafka.client import KafkaClient

if __name__ == '__main__':
    kafka = KafkaClient("kafka01", 9092)

    for i in range(100000):
        kafka.send_messages_simple("test", "some message {0}".format(i))
    kafka.close()
