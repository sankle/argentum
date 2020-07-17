#include <math.h>

#include <algorithm>
#include <string>
//-----------------------------------------------------------------------------
#include "../../includes/Model/Character.h"
#include "../../includes/Model/Formulas.h"
//-----------------------------------------------------------------------------
#include <iostream>  //sacar
//-----------------------------------------------------------------------------
#define DEFAULT_MOVING_ORIENTATION DOWN_ORIENTATION
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

Character::Character(const CharacterCfg& init_data, const RaceCfg& race,
                     const KindCfg& kind, LogicMaps& logic_maps,
                     const Id init_map, const int init_x_coord,
                     const int init_y_coord, ItemsContainer& items_container,
                     Formulas& formulas, const int& rate,
                     const unsigned int critical_attack_dmg_modifier,
                     const unsigned int ms_to_update_attributes)
    : intelligence(kind.intelligence + race.intelligence),
      constitution(kind.constitution + race.constitution),
      strength(kind.strength + race.strength),
      agility(kind.agility + race.agility),

      nickname(init_data.nickname),

      race(race, init_data.head_id, init_data.body_id),
      kind(kind),

      state(StateFactory::newState(init_data.state, this->race)),

      level(init_data.level, init_data.exp, formulas),

      inventory(init_data.inventory, init_data.safe_gold, init_data.excess_gold,
                this->level, items_container, formulas),
      equipment(init_data.equipment, items_container),

      position(init_map, init_x_coord, init_y_coord, logic_maps),

      is_meditating(false),

      is_moving(false),
      moving_cooldown(0),
      attribute_update_time_elapsed(0),

      attack_cooldown(0),
      broadcast(false),

      formulas(formulas),

      rate(rate),
      critical_attack_dmg_modifier(critical_attack_dmg_modifier),
      ms_to_update_attributes(ms_to_update_attributes) {
    this->updateLevelDependantAttributes();  // Set max_health, max_mana,
                                             // max_inventory_gold.
    if (init_data.new_created) {
        health = max_health;
        mana = max_mana;
    } else {
        health = init_data.health;
        mana = init_data.mana;
    }
}

Character::~Character() {
    delete state;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Actualización de atributos
//-----------------------------------------------------------------------------

void Character::act(const unsigned int it) {
    if (is_moving) {
        _updateMovement(it);
    } else {
        if (moving_cooldown > 0) {
            moving_cooldown = std::max((int)(moving_cooldown - it * rate), 0);
        }
    }

    if (attack_cooldown > 0)
        attack_cooldown = std::max((int)(attack_cooldown - it * rate), 0);

    _updateTimeDependantAttributes(it);
}

void Character::updateLevelDependantAttributes() {
    this->max_health = formulas.calculateMaxHealth(
        this->constitution, this->kind.max_health_factor,
        this->race.max_health_factor, this->level.getLevel());

    this->max_mana = formulas.calculateMaxMana(
        this->intelligence, this->kind.max_mana_factor,
        this->race.max_mana_factor, this->level.getLevel());

    this->inventory.updateMaxAmountsOfGold();

    this->broadcast = true;
}

void Character::_updateTimeDependantAttributes(const unsigned int it) {
    attribute_update_time_elapsed += it * rate;

    while (attribute_update_time_elapsed >= ms_to_update_attributes) {
        unsigned int health_update = formulas.calculateHealthTimeRecovery(
            this->race.health_recovery_factor, ms_to_update_attributes);

        unsigned int mana_update = formulas.calculateManaTimeRecovery(
            this->race.mana_recovery_factor, ms_to_update_attributes);

        if (this->is_meditating)
            mana_update += formulas.calculateManaMeditationTimeRecovery(
                this->kind.meditation_factor, this->intelligence,
                ms_to_update_attributes);

        if ((this->health < this->max_health))
            this->recoverHealth(health_update);
        if ((this->mana < this->max_mana))
            this->recoverMana(mana_update);

        attribute_update_time_elapsed -= ms_to_update_attributes;
    }
}

void Character::_updateMovement(const unsigned int it) {
    this->moving_cooldown -= it * rate;

    while (this->moving_cooldown <= 0) {
        this->broadcast = true;

        this->moving_cooldown += UNIT_TIME_TO_MOVE;

        this->position.move(false);
    }
}

std::string Character::getNickname() {
    return nickname;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Movimiento
//-----------------------------------------------------------------------------

void Character::startMovingUp() {
    this->state->move();
    this->is_meditating = false;
    this->position.changeOrientation(UP_ORIENTATION);
    this->is_moving = true;
}

void Character::startMovingDown() {
    this->state->move();
    this->is_meditating = false;
    this->position.changeOrientation(DOWN_ORIENTATION);
    this->is_moving = true;
}

void Character::startMovingRight() {
    this->state->move();
    this->is_meditating = false;
    this->position.changeOrientation(RIGHT_ORIENTATION);
    this->is_moving = true;
}

void Character::startMovingLeft() {
    this->state->move();
    this->is_meditating = false;
    this->position.changeOrientation(LEFT_ORIENTATION);
    this->is_moving = true;
}

void Character::stopMoving() {
    this->is_moving = false;
}

void Character::teleport(const Id map_id, const uint32_t x_coord,
                         const uint32_t y_coord) {
    this->position.teleport(map_id, x_coord, y_coord);
    this->broadcast = true;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Modificación de maná y vida.
//-----------------------------------------------------------------------------

const bool Character::recoverHealth(unsigned int& points) {
    if (!points || !health || this->health == this->max_health) {
        points = 0;
        return false;
    }

    if (health + points > max_health) {
        points = max_health - health;
        health = max_health;
    } else {
        health += points;
    }

    this->broadcast = true;
    return true;
}

const bool Character::recoverMana(unsigned int& points) {
    if (!points || !health || this->mana == this->max_mana)
        return false;

    if (mana + points > max_mana) {
        points = max_mana - mana;
        mana = max_mana;
    } else {
        mana += points;
    }

    this->broadcast = true;
    return true;
}

void Character::heal() {
    this->state->beHealed();
    this->health = max_health;
    this->mana = max_mana;
    this->broadcast = true;
}

void Character::consumeMana(const unsigned int points) {
    if (this->mana < points) {
        throw InsufficientManaException();
    }

    this->mana -= points;
    this->broadcast = true;
}

void Character::meditate() {
    this->state->meditate();
    this->kind.meditate();
    this->is_meditating = true;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Manejo de items.
//-----------------------------------------------------------------------------

void Character::equip(unsigned int inventory_position) {
    unsigned int amount = 1;
    Item* item_to_equip =
        this->inventory.gatherItem(inventory_position, amount);

    if (!item_to_equip)
        return;

    item_to_equip->equip(*this);

    this->broadcast = true;
}

void Character::equip(Wearable* item) {
    Wearable* prev_equipped_item = this->equipment.add(item);
    if (prev_equipped_item) {
        // Agrego lo que tenia equipado al inventario.
        takeItem(prev_equipped_item);
    }

    this->broadcast = true;
}

void Character::unequip(unsigned int n_slot) {
    Wearable* unequipped_item = this->equipment.remove(n_slot);

    if (!unequipped_item)
        return;

    try {
        this->inventory.addItem((Item*)unequipped_item, 1);
    } catch (FullInventoryException()) {
        // No se pudo agregar al inventario => lo devolvemos al equipment
        this->equipment.add(unequipped_item);
        throw FullInventoryException();
    }

    this->broadcast = true;
}

void Character::takeItem(Item* item, unsigned int amount) {
    this->state->takeItem();
    this->broadcast = true;
    this->inventory.addItem(item, amount);
}

Item* Character::dropItem(const unsigned int n_slot, unsigned int& amount) {
    Item* dropped_item = this->inventory.gatherItem(n_slot, amount);

    if (dropped_item)
        this->broadcast = true;

    return dropped_item;
}

void Character::gatherGold(const unsigned int amount) {
    this->state->gatherGold();
    this->broadcast = true;
    this->inventory.gatherGold(amount);
}

void Character::takeGold(unsigned int& amount) {
    this->state->takeItem();
    this->broadcast = true;
    this->inventory.addGold(amount);
}

void Character::dropAllItems(std::vector<DroppingSlot>& dropped_items) {
    this->equipment.dropAll(dropped_items);
    this->inventory.dropAll(dropped_items);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Ataque y defensa.
//-----------------------------------------------------------------------------

void Character::setAttackCooldown(const unsigned int cooldown) {
    this->attack_cooldown += cooldown;
}

void Character::beAttacked() {
    // Delego en mi estado si puedo ser atacado.
    this->state->beAttacked();
}

void Character::doMagic() {
    // Delego en mi kind si puedo hacer magia.
    this->kind.doMagic();
}

void Character::_checkPriorToUseWeaponConditions(Attackable* target) const {
    // Delego si puedo atacar en mi estado.
    this->state->attack();

    // Verifico cooldown de arma de ataque.
    if (this->attack_cooldown)
        throw AttackCooldownTimeNotElapsedException();

    // Verifico si alguno atacante o atacado está en zona segura.
    if (this->position.isInSafeZone() || target->getPosition().isInSafeZone())
        throw CantAttackInSafeZoneException();

    // Nos fijamos si el atacado está en el rango del arma.
    // Si no lo está, lanzamos excepción.
    const unsigned int weapon_range = this->equipment.getAttackRange();
    const unsigned int distance =
        this->position.getDistance(target->getPosition());

    if (distance > weapon_range)
        throw OutOfRangeAttackException();
}

void Character::_checkPriorToUseAttackWeaponConditions(
    Attackable* target) const {
    // Verificación de que no se quiera hacer daño a sí mismo
    if (this == target)
        throw CantAttackItselfException();

    // Verificación si dado el estado del otro jugador se puede atacarlo.
    target->beAttacked();

    // Verificación de si el jugador es newbie.
    if (target->isNewbie())
        throw NewbiesCantBeAttackedException();

    if (this->isNewbie() && !target->isCreature())
        throw NewbiesCantAttackCharactersException();

    // Verificación de diferencia de niveles entre jugadores.
    const unsigned int target_level = target->getLevel();

    if (!target->isCreature() &&
        !(formulas.canAttackByLevel(this->level.getLevel(), target_level)))
        throw TooHighLevelDifferenceOnAttackException();
}

const bool Character::_useHealingWeapon(Attackable* target, int& damage) {
    // Si el arma es curativa, obtengo sus puntos y curo.
    unsigned int healing_points = this->equipment.useAttackItem(*this);
    target->recoverHealth(healing_points);

    // Establezco valor negativo de daño -> puntos curativos.
    damage = -healing_points;
    return false;
}

const bool Character::_useAttackWeapon(Attackable* target, int& damage) {
    _checkPriorToUseAttackWeaponConditions(target);

    // Obtenemos el danio del arma.
    damage = this->equipment.useAttackItem(*this);

    // Se define si el ataque es critico
    bool critical_attack = formulas.isCriticalAttack();
    if (critical_attack)
        damage = damage * critical_attack_dmg_modifier;

    // El atacado recibe el daño del ataque.
    const bool eluded = !target->receiveAttack(damage, critical_attack);

    // Sumamos exp. de ataque.
    this->level.onAttackUpdate(*this, damage, target->getLevel());

    // Si murió, sumamos la exp. de muerte.
    if (!target->getHealth())
        this->level.onKillUpdate(*this, target->getMaxHealth(),
                                 target->getLevel());

    this->broadcast = true;

    return eluded;
}

const bool Character::useWeapon(Attackable* target, int& damage) {
    _checkPriorToUseWeaponConditions(target);

    this->is_meditating = false;

    if (this->equipment.getWeaponType() == HEALING)
        return _useHealingWeapon(target, damage);

    // Es un arma de ataque.
    return _useAttackWeapon(target, damage);
}

const bool Character::receiveAttack(int& damage, const bool eludible) {
    this->is_meditating = false;

    if (eludible && formulas.isAttackEluded(this->agility)) {
        // El ataque es esquivado, no se recibe daño
        damage = 0;
        return false;
    }

    const unsigned int defense_points = this->equipment.getDefensePoints(*this);

    damage = std::max(0, (int)(damage - defense_points));

    this->health = std::max(0, (int)(this->health - damage));

    if (this->health == 0)
        this->die();

    this->broadcast = true;

    return true;
}

void Character::die() {
    delete this->state;
    this->state = new Dead(this->race.dead_head_id, this->race.dead_body_id);

    this->mana = 0;

    this->broadcast = true;
}

void Character::resurrect() {
    this->state->resurrect();

    delete this->state;
    this->state = new Alive(this->race.head_id, this->race.body_id);

    this->heal();

    this->broadcast = true;
}

void Character::startResurrecting() {
    this->state->resurrect();

    this->stopMoving();

    delete this->state;
    this->state =
        new Resurrecting(this->race.dead_head_id, this->race.dead_body_id);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Obtención de estado.
//-----------------------------------------------------------------------------

const Position& Character::getPosition() const {
    return this->position;
}

const unsigned int Character::getLevel() const {
    return this->level.getLevel();
}

const unsigned int Character::getHealth() const {
    return this->health;
}

const unsigned int Character::getMaxHealth() const {
    return this->max_health;
}

const bool Character::isNewbie() const {
    return this->level.isNewbie();
}

const Id Character::getMapId() const {
    return this->position.getMapId();
}

const std::string& Character::getNickname() const {
    return this->nickname;
}

const bool Character::isCreature() const {
    return false;
}

const WeaponType Character::getWeaponType() const {
    return this->equipment.getWeaponType();
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Control de broadcast.
//-----------------------------------------------------------------------------

const bool Character::mustBeBroadcasted() const {
    return this->broadcast;
}

void Character::beBroadcasted() {
    this->broadcast = false;
    this->position.beBroadcasted();
}

void Character::fillBroadcastData(PlayerData& data) const {
    // Llena map_id, x_tile, y_tile, orientation.
    this->position.fillBroadcastData(data.basic_data);
    this->state->fillBroadcastData(data);

    data.basic_data.movement_speed = UNIT_MOVEMENT_SPEED;
    data.is_shorter = race.is_shorter;
    data.nickname = this->nickname;
    data.health = this->health;
    data.max_health = this->max_health;
    data.mana = this->mana;
    data.max_mana = this->max_mana;

    // Llena level, exp, levelup_exp
    this->level.fillBroadcastData(data);
    // Llena safe_gold, excess_gold e inventory.
    this->inventory.fillBroadcastData(data);
    // Llena equipment, helmet_id, armour_id, shield_id, weapon_id;
    this->equipment.fillBroadcastData(data);
}

void Character::fillPersistenceData(CharacterCfg& data) const {
    // llenar map x_tile, y_tile;
    this->position.fillPersistenceData(data);

    data.head_id = this->race.head_id;
    data.body_id = this->race.body_id;

    std::strncpy(data.nickname, nickname.c_str(), sizeof(data.nickname) - 1);

    data.race = this->race.id;
    data.kind = this->kind.id;

    // aca verifica el estado del character, hay que mejorar!
    if (!this->health) {
        data.state = DEAD;
    } else {
        data.state = ALIVE;
    }

    this->level.fillPersistenceData(data);
    this->equipment.fillPersistenceData(data);
    this->inventory.fillPersistenceData(data);

    data.health = this->health;
    data.mana = this->mana;
    data.new_created = false;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

const char* InsufficientManaException::what() const noexcept {
    return "No tienes suficiente maná.";
}

const char* CantAttackInSafeZoneException::what() const noexcept {
    return "No se puede atacar o lanzar hechizos en una zona segura.";
}

const char* CantAttackItselfException::what() const noexcept {
    return "No puedes hacerte daño a ti mismo.";
}

const char* OutOfRangeAttackException::what() const noexcept {
    return "El objetivo al que quieres atacar está fuera del rango de tu arma.";
}

const char* NewbiesCantBeAttackedException::what() const noexcept {
    return "No puedes atacar a jugador newbie.";
}

const char* NewbiesCantAttackCharactersException::what() const noexcept {
    return "No puedes atacar a otro jugador. Eres newbie.";
}

const char* AttackCooldownTimeNotElapsedException::what() const noexcept {
    return "No puedes usar el arma tan rápido, la misma está en enfriamiento.";
}

const char* TooHighLevelDifferenceOnAttackException::what() const noexcept {
    return "La diferencia de niveles es mayor a 12.";
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

void Character::debug() {
    std::cout << "**Character debug:**" << std::endl;
    // this->inventory.debug();
    // this->equipment.debug();
    std::cout << "health: " << this->health << std::endl;
    std::cout << "mana: " << this->mana << std::endl;
    std::cout << "position: x = " << this->position.getX()
              << " y = " << this->position.getY() << std::endl;
}

//-----------------------------------------------------------------------------
