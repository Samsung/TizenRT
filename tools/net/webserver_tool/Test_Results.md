# Results

### Case 1: HTTPS, chunk-size 100B, data-size 1KB, keep-alive 0 :

| Test_Type | Total_checks | Pass | Fail |
| ------|------ | ------|------ |
|    GET    |     1000     | 1000 |  0   |
|    POST   |     1000     | 1000 |  0   |
|    PUT    |     1000     | 1000 |  0   |
|   DELETE  |     1000     | 1000 |  0   |


### Case 2: HTTPS,Keep-alive 0 , chunk 0 :
| Test_Type | Total_checks | Pass | Fail |
| ------|------ | ------|------ |
|    GET    |     1000     | 991  |  9  |
|    POST   |     1000     | 1000 |  0   |
|    PUT    |     1000     | 1000 |  0   |
|   DELETE  |     1000     | 993  |  7  |

### Case 3: HTTPS, Chunk 1KB, 1MB, keep-alive 0 :

|    Test_Type     | Total_checks | Pass | Fail |
| ------|------ | ------|------ |
| POST |     1000     | 1000 |  0   |

### Case 4: HTTP, Chunk 0 keep-alive 1,(5 requests per connection) :

|    Test_Type     | Total_checks | Pass | Fail |
| ------|------ | ------|------ |
| GET |     50     | 50 |  0   |

### Case 5: HTTP, chunk-size 100B, data-size 1KB, keep-alive 1,(5 requests per connection) :

|    Test_Type     | Total_checks | Pass | Fail |
| ------|------ | ------|------ |
| GET |     50     | 50 |  0   |
