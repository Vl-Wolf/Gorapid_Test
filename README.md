[English](#test-task-for-unreal-engine-programmer-position-at-gorapid-studio)  
[Русский](#тестовое-задание-на-позицию-unreal-engine-программиста-в-гоурэпид-студио)

---

# Test task for Unreal Engine programmer position at Gorapid Studio

## Task

- The player and the enemy should have a health counter, the ability to take and deal damage. The enemy should die when the health counter expires, but such a mechanic is not mandatory for the player.
- The enemy should have a weapon with a limited ammo capacity in the magazine/clip. The total ammo supply for the enemy is not important.  
Tasks for the enemy:
  - Detect the enemy (player) and switch from a waiting mode to an aggressive mode.
  - Precisely shoot at the enemy with the weapon.
  - Find cover for safe firing. Fire from the spot if there is no cover.
  - Find and retreat to cover for weapon reloading.
  - Have a health counter and die if the health supply runs out.

## Implementation

1. The following C++ classes and their Blueprint derivatives were created:
   - Actor:
     - Character, where the logic of weapon control and interaction with the inventory is outlined.
     - Enemy, which replicates the character's logic.
     - Weapon, where the logic of projectile release is outlined.
     - Projectile, where the logic of causing damage is outlined.
   - Actor Component:
     - Health
     - Inventory
   - Controller:
     - Player controller, where the logic of character control is outlined.
     - Enemy controller, where the logic of changing variables necessary for the behavior tree is outlined.
2. Blueprint classes of the behavior tree and collectible items were also created.
3. Tasks for the behavior tree were created:
    - Aim at the character.
    - Attack the character.
    - Stop attacking the character.
    - Move to cover.
    - Reload the weapon.
    - Exit cover.
    - Lose the character.
4. EQS (Environment Query System) patrols were created for patrolling, finding cover for firing, and finding cover for reloading.

## Result

#### Patrolling

![Patrolling](Assets/Patrolling.gif)

#### Attack

![Attack](Assets/Attack.gif)

#### MoveToCover

![MoveToCover](Assets/MoveToCover.gif)

#### Reloading

![Reload](Assets/Reload.gif)

#### Death

![Death](Assets/Death.gif)


# Тестовое задание на позицию Unreal Engine программиста в Гоурэпид Студио

## Задание

- Игрок и противник должны обладать счетчиком здоровья, способностью принимать и наносить урон. Противник должен умирать по истечении счетчика здоровья, для игрока такая механика не обязательна.
- Противник должен иметь оружие с ограниченным запасом патрон в магазине\обойме. Общий запас патронов у противника не важен.  
Задачи по противнику:
  - Заметить врага (игрока) и перейти из режима ожидания в агрессивный режим.
  - Прицельно стрелять в противника из оружия.
  - Находить укрытие для безопасного ведения огня. Вести огонь с места если укрытия нет.
  - Находить и отступать в укрытие для перезарядки оружия.
  - Обладать счетчиком здоровья и умирать в случае если запас здоровья заканчивается.

## Реализация

1. Были созданы следующие С++ классы и наследники от них Blueprint:
   - Actor:
     - Персонаж, в котором расписана логика управления оружием, взаимодействие с инвентарём.
     - Противник, который повторяет логику персонажа.
     - Оружие, в котором расписана логика выпуска снаряда.
     - Патроны, в котором расписана логика нанесения урона.
   - Actor Component:
     - Здоровье.
     - Инвентарь.
   - Controller:
     - Контроллер персонажа, в котором расписана логика управления персонажа.
     - Контроллер противника, в котором расписана логика изменения переменных необходимых для дерева поведения.

2. Были созданы Blueprint классы дерева поведения и подбираемых предметов.
3. Для дерева поведения были созданы следующие задачи:
   - Прицелиться на персонажа.
   - Атаковать персонажа.
   - Перестать атаковать персонажа.
   - Идти за укрытие.
   - Перезарядить оружие.
   - Выйти из укрытия
   - Потерять персонажа
4. Так же были созданы EQS (Environment Query System) патрулирования, поиска укрытия для ведения огня, поиск укрытия для перезарядки

## Итог

#### Патрулирование

![Attack](Assets/Patrolling.gif)

#### Атака

![Patrolling](Assets/Attack.gif)

#### Поиск укрытия 

![MoveToCover](Assets/MoveToCover.gif)

#### Перезарядка

![Reload](Assets/Reload.gif)

#### Смерть

![Death](Assets/Death.gif)
