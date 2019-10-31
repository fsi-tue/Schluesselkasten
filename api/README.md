# API

UIDofCard: Hexadecimal value of UID, lowercase, without :

Name: 3-40 arbitrary characters

RegEx of user.csv entrys: ```/[a-f0-9]{8};.{3,40}/```
## Get ID-List
```POST``` to api.php with:

```
apikey=YourSecretApikey
```
Will return:
```
NumberOfEntrys(integer)
UIDofCard1
UIDofCard2
...
UIDofCardN
```
### Possible Errors
- ```Error: Wrong API Key``` The submitted API-Key does not match the saved one.

## Get ID-List with names:
```POST``` to api.php with:

```
apikey=YourSecretApikey
action=getall
```
Will return:

```
NumberOfEntrys(integer)
UIDofCard1;Name1
UIDofCard2;Name2
...
UIDofCardN;NameN

```
### Possible Errors
- ```Error: Wrong API Key``` The submitted API-Key does not match the saved one.


## Create User:
```POST``` to api.php with:

```
apikey=YourSecretApikey
action=create
usr=UIDofCard;Name
```
Will return: ```Created UIDofCard;Name```

### Possible Errors
- ```Error: Wrong API Key``` The submitted API-Key does not match the saved one.
- ```Error: Wrong USR pattern.``` The submitted usr field does not match the UIDofCard;Name pattern.
- ```Error: User is already in file.``` The submitted UID is already in the user.csv.

## Delete User:
```POST``` to api.php with:
```
apikey=YourSecretApikey
action=delete
usr=UIDofCard;Name
```
Will return: ```Deleted UIDofCard;Name```

### Possible Errors
- ```Error: Wrong API Key``` The submitted API-Key does not match the saved one.
- ```Error: Wrong USR pattern.``` The submitted usr field does not match the UIDofCard;Name pattern.
- ```Error: User not found.``` The submitted User does not exist in the user.csv.. Beware! UID and name must match exactly the one in the file.


# Examples
## getall
```
POST /api.php HTTP/1.0
Host: sub.domain.tld
Content-Type: application/x-www-form-urlencoded

apikey=YourSecretAPIKey&action=getall
```

Will return:
```
3
ab7a00f3;JonDoe
0bba32f3;JaneDoe
effa2af3;MaxMuster
```

## create

```
POST /api.php HTTP/1.0
Host: sub.domain.tld
Content-Type: application/x-www-form-urlencoded

apikey=YourSecretAPIKey&action=create&usr=ab7a00f3;JoeJustMarried
```
Will return ``` Error: User is already in file. ``` because the UID is already saved in user.csv

If the UID is not already saved this POST will return: ```Created ab7a00f3;JoeJustMarried```

