from storages.backends.s3boto import S3BotoStorage

StaticRootS3BotoStorage = lambda: S3BotoStorage(location='static') #bucketname / static
MediaRootS3BotoStorage  = lambda: S3BotoStorage(location='media') #bucketname / media