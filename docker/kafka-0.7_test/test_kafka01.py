from kafka.client import KafkaClient
from kafka.queue import KafkaQueue
import nose

def test_kafka_queue():
    kafka = KafkaClient("kafka01", 9092)
    q = KafkaQueue(kafka, "queue", [0])
    q.put("first")
    q.put("second")
    assert q.get() == "first"
    assert q.get() == "second"
    q.close()
    kafka.close()
