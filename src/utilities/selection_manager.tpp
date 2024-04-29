template<class T>
SelectionManager<T>::SelectionManager():
	currentSelection(SELECTION_NOT_SET) {}

template<class T>
SelectionManager<T>::SelectionManager(const std::vector<T>& selections) :
	selections(selections), currentSelection(0) {}

template<class T>
void SelectionManager<T>::next(void) const noexcept {
	if (currentSelection == SELECTION_NOT_SET)
		return;
	currentSelection = (currentSelection + 1) % (int)selections.size();
}

template<class T>
void SelectionManager<T>::prev(void) const noexcept {
	if (currentSelection == SELECTION_NOT_SET)
		return;
	int size = (int)selections.size();
	currentSelection = (currentSelection + size - 1) % size;
}

template<class T>
void SelectionManager<T>::set(int newSelection) const {
	if (
		newSelection != SELECTION_NOT_SET and
		not (0 <= newSelection and newSelection < selections.size())
	) {
		throw std::out_of_range(std::format(
			"In SelectionManager::set(): Invalid selection ID \'{}\' received.\n"
			"\t The ID should be in the range [0, {}) or {}.",
			newSelection, size(), SELECTION_NOT_SET
		));
	}
	currentSelection = newSelection;
}

template<class T>
size_t SelectionManager<T>::size(void) const noexcept {
	return selections.size();
}

template<class T>
void SelectionManager<T>::add(T newSelection) noexcept {
	if (currentSelection == SELECTION_NOT_SET)
		currentSelection = 0;
	selections.push_back(newSelection);
}

template<class T>
void SelectionManager<T>::remove(size_t selectionId) {
	if (not (0 <= selectionId and selectionId < selections.size())) {
		throw std::out_of_range(std::format(
			"In SelectionManager::remove(): Invalid selection ID \'{}\' received.\n"
			"\t The ID should be in the range [0, {}).",
			selectionId, size()
		));
	}
	selections.erase(selections.begin() + selectionId);
	if (currentSelection >= selectionId)
		currentSelection--;
}

template<class T>
T SelectionManager<T>::get(void) const {
	if (currentSelection == SELECTION_NOT_SET)
		throw std::logic_error("In SelectionManager::get(): The selection is not set");
	return selections[currentSelection];
}

template<class T>
int SelectionManager<T>::getSelectionId(void) const noexcept {
	return currentSelection;
}