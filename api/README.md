# API

UIDofCard: Hexadecimal value of UID, lowercase, without :

Name: 3-40 arbitrary characters

RegEx of user.csv entrys: ```/[a-f0-9]{8};.{3,40}/```
## Get ID-List
Post to api.php with:

```
apikey:YourSecretApikey
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
Post to api.php with:

```
apikey:YourSecretApikey
action:getall
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
Post to api.php with:

```
apikey:YourSecretApikey
action:create
usr:UIDofCard;Name
```
Will return: ```Created UIDofCard;Name```

### Possible Errors
- ```Error: Wrong API Key``` The submitted API-Key does not match the saved one.
- ```Error: Wrong USR pattern.``` The submitted usr field does not match the UIDofCard;Name pattern.
- ```Error: User is already in file.``` The submitted UID is already in the user.csv.

## Delete User:
Post to api.php with:
```
apikey:YourSecretApikey
action:delete
usr:UIDofCard;Name
```
Will return: ```Deleted UIDofCard;Name```

### Possible Errors
- ```Error: Wrong API Key``` The submitted API-Key does not match the saved one.
- ```Error: Wrong USR pattern.``` The submitted usr field does not match the UIDofCard;Name pattern.
- ```Error: User not found.``` The submitted User does not exist in the user.csv.. Beware! UID and name must match exactly the one in the file.