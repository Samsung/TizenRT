# Results

### Case 1: HTTP, keep-alive OFF , Chunk-transfer OFF :
Command : `webserver_test 1000 [Test_Type] http://127.0.0.1/ test_entity=100`

| Test_Type | Total_checks | Pass | Fail |
| ------|------ | ------|------ |
|    GET    |     1000     | 1000 |  0   |
|    POST   |     1000     | 1000 |  0   |
|    PUT    |     1000     | 1000 |  0   |
|   DELETE  |     1000     | 1000 |  0   |

### Case 2: HTTP, chunk-count 10, test-entity 100 (total=1kb), keep-alive OFF :
Command : `webserver_test 1000 [Test_Type] http://127.0.0.1/ test_entity=100 chunk-count=10`

| Test_Type | Total_checks | Pass | Fail |
| ------|------ | ------|------ |
|    POST   |     1000     | 1000 |  0   |
|    PUT    |     1000     | 1000 |  0   |

### Case 3: HTTP, chunk-count 10, test-entity 100 (total=1kb), keep-alive 5 :
Command : `webserver_test 1000 [Test_Type] http://127.0.0.1/ test_entity=100 chunk-count=10 keep-alive=5`

| Test_Type | Total_checks | Pass | Fail |
| ------|------ | ------|------ |
|    POST   |     1000     | 988 |  12   |
|    PUT    |     1000     | 953 |  47   |

### Case 4: HTTPS,Keep-alive 0 , chunk 0 :
Command : `webserver_test 200 [Test_Type] https://127.0.0.1/ test_entity=100`

| Test_Type | Total_checks | Pass | Fail |
| ------|------ | ------|------ |
|    GET    |     200     |  200 |  0  |
|    POST   |     200     | 200 |  0   |
|    PUT    |     200     | 200 |  0   |
|   DELETE  |     200     |  200 |  0  |

### Case 5: HTTPS, chunk-count 10, test-entity 100 (total=1kb), keep-alive OFF :
Command : `webserver_test 200 [Test_Type] https://127.0.0.1/ test_entity=100 chunk-count=10`

| Test_Type | Total_checks | Pass | Fail |
| ------|------ | ------|------ |
|    POST   |     200     | 200 |  0   |
|    PUT    |     200     | 200 |  0   |

### Case 6: HTTPS, chunk-count 10, test-entity 100 (total=1kb), keep-alive 5 :
Command : `webserver_test 200 [Test_Type] https://127.0.0.1/ test_entity=100 chunk-count=10 keep-alive=5`

| Test_Type | Total_checks | Pass | Fail |
| ------|------ | ------|------ |
|    POST   |     200     | 200 |  0   |
|    PUT    |     200     | 200 |  0   |

### Case 7: HTTPS, chunk-count 1000, test-entity 1000 (total=1mb), keep-alive OFF :
Command : `webserver_test 200 [Test_Type] https://127.0.0.1/ test_entity=1000 chunk-count=1000`

| Test_Type | Total_checks | Pass | Fail |
| ------|------ | ------|------ |
|    POST   |     200     | 200 |  0   |
|    PUT    |     200     | 200 |  0   |

### Case 8: HTTPS, chunk-count 10, test-entity 100 (total=1kb), trailer-header ON :
Command : `webserver_test 200 [Test_Type] https://127.0.0.1/ test_entity=100 chunk-count=10 trailer=1`

| Test_Type | Total_checks | Pass | Fail |
| ------|------ | ------|------ |
|    POST   |     200     | 200 |  0   |
|    PUT    |     200     | 200 |  0   |
