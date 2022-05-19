#pragma once

#include "ParamDlgCard.h"


class ParamDlgCardShared : public ParamDlgCard
{
private:
	/*
	* Defines the resource assoicated with this class.
	*/
	enum { IDD = PARAMETERS_CARD_SHARED };

	/**
	* Handles the pressing of the "Manage Parameter Localy" button
	* 
	* Passes a message up to the ParamDlgScrollablePane
	*/
	afx_msg void OnMakeLocal();

public:

	/**
	* Standard constructor
	*/
	ParamDlgCardShared(ClassPar* param, CWnd* pParent);

	/**
	* Creates the dialog object and attaches it to this object.
	* 
	* Must be called directly after the constructor.
	*/
	virtual void call_create(CWnd* pParent) override;
	
	/**
	* Renders the value grid for the parameter
	*/
	virtual void RenderGrid() override;

	DECLARE_MESSAGE_MAP()
};

