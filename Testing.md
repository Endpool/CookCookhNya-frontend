# Manual Testing scenarious

1.
    - Given: Nothing
    - When: `/start` -> "*🆕Создать*" -> *name_of_storage*
    - Then: New storage with name *name_of_storage* in the list of storages

2.
    - Given: At least one existing storage with name *name_of_storage*
    - When: `/start` -> "*🚮Удалить*" -> *name_of_storage*
    - Then: Storage with name *name_of_storage* is removed from storage list

3.
    - Given: At least one storage with name *name_of_storage* and some ingredients
    - When: `/start` -> *name_of_storage* -> "*Продукты🍗*"
    - Then: The list of ingredients in storage is showed

4.
    - Given: At least one storage with name *name_of_storage*
    - When: `/start` -> *name_of_storage* -> "*👥Участники*"
    - Then: The list of members of *name_of_storage* is shown

5.
    - Given: At least one storage with name *name_of_storage*
    - When: `/start` -> *name_of_storage* -> "*👥Участники*" -> "*🔐Добавить*" -> *forward message of needed user*
    - Then: New member is added to *name_of_storage*, now he/she can operate with its ingredients

6.
    - Given: At least one storage with name *name_of_storage*
    and with at least one member with *name of member* in it
    - When: `/start` -> *name_of_storage* -> "*👥Участники*" -> "*🔒Удалить*" -> *name of member*
    - Then: Member *name of member* is deleted from *name_of_storage*, has no access to it

7.
    - Given: At least one storage with names *name_of_storage1*, *name_of_storage2*...,
    at least one existing recipe in system, at least one existing ingredient in system
    - When: `/start` -> "*🤤Xочу кушать!*" -> *name_of_storage1* -> *name_of_storage2*
    -> (until all storages which user wants to use are selected) -> "*▶️Подтвердить*"
    - Then: List of recipes with info about amount of ingredients which are present in recipe in chosen storages

8.
    - Given: At least one storage with name *name_of_storage*, at least one existing recipe in system,
    at least one existing ingredient in system and in *name_of_storage*
    - When: `/start`  -> *name_of_storage* -> "*🤤Xочу кушать!*"
    - Then: List of recipes with info about amount of ingredients which are present in recipe in chosen storages

9.
    - Given: At least one storage with names *name_of_storage1*, *name_of_storage2*...,
    at least one existing recipe in system, at least one existing ingredient in system
    - When: (`/start` -> "*🤤Xочу кушать!*" -> (choose storages) OR `/start` -> *name_of_storage*
    -> "*🤤Xочу кушать!*" -> *recipe name*
    - Then: The list of ingredients which are present/not present in chosen storages
    and ingredients marked with "*?*",
    which means that user has storages where that ingredient is present but it's not chosen yet

10.
    - Given: At least one storage with names *name_of_storage1*, *name_of_storage2*...,
    at least one existing recipe in system, at least one existing ingredient in system
    and at least one storage which wasn't chosen but has ingredient which is not present in selected storages.
    - When: (`/start` -> "*🤤Xочу кушать!*" -> (choose storages) OR `/start` -> *name_of_storage*
    -> "*🤤Xочу кушать!*" -> *recipe name* -> "*?*"
    - Then: The list of storages which user may add to "*chosen*" storages to make some ingredients present
