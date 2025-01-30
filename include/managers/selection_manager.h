#pragma once

#include <vector>
#include <stdexcept>
#include <format>

// TODO: Complete SelectionManager.

template<class T>
class SelectionManager {
private:
	std::vector<T> selections;
	mutable int currentSelection; // mutable: this field should ALWAYS be modifiable.
public:
	static const int SELECTION_NOT_SET = -1;

	SelectionManager();
	SelectionManager(const std::vector<T>& selections);

	/// <summary>
	/// Set to next selection.
	/// </summary>
	void next(void) const noexcept;

	/// <summary>
	/// Set to previous selection.
	/// </summary>
	void prev(void) const noexcept;

	/// <summary>
	/// Set current selection ID to @param newSelection.
	/// Throws std::out_of_range if ID is not in range of [0, size) or SELECTION_NOT_SET.
	/// </summary>
	/// <param name="newSelection">The new selection ID.</param>
	void set(int newSelection) const;

	/// <summary>
	/// Gets the count of available selections.
	/// </summary>
	/// <returns>The count of available selections.</returns>
	size_t size(void) const noexcept;

	/// <summary>
	/// Adds @param newSelection to the manager.
	/// </summary>
	/// <param name="newSelection">The new selection.</param>
	void add(T newSelection) noexcept;

	/// <summary>
	/// Removes the selection at @param selectionId.
	/// Throws std::out_of_range if selectionId is inavlid.
	/// </summary>
	/// <param name="selectionId">The position of where the selection is at.</param>
	void remove(size_t selectionId);
	
	/// <summary>
	/// Gets the current selection.
	/// Throws std::logic_error is current selection is SELECTION_NOT_SET.
	/// </summary>
	/// <returns>The current selection.</returns>
	T get(void) const;

	/// <summary>
	/// Gets the current selection ID.
	/// </summary>
	/// <returns>The current selection ID.</returns>
	int getSelectionId(void) const noexcept;
};

#include "../../src/utilities/selection_manager.hpp"