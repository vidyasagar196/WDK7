#pragma once

#include "resource.h"
#include "WpdMultiTransportDriver.h"

class ATL_NO_VTABLE CDevice :
    public CComObjectRootEx<CComMultiThreadModel>,
    public IPnpCallback,
    public IPnpCallbackSelfManagedIo,
    public IPnpCallbackHardware
{
public:
    CDevice() :
        m_pWpdBaseDriver(NULL)
    {
    }

    DECLARE_NOT_AGGREGATABLE(CDevice)

    BEGIN_COM_MAP(CDevice)
        COM_INTERFACE_ENTRY(IPnpCallback)
        COM_INTERFACE_ENTRY(IPnpCallbackSelfManagedIo)
        COM_INTERFACE_ENTRY(IPnpCallbackHardware)
    END_COM_MAP()

public:
    static HRESULT
    CreateInstance(
        IUnknown**              ppUnkwn,
        IWDFDeviceInitialize*   pDeviceInit,
        WpdBaseDriver*          pWpdBaseDriver)
    {
        *ppUnkwn = NULL;

        //
        // Set device properties.
        //
        pDeviceInit->SetLockingConstraint(None);

        CComObject< CDevice> *pMyDevice = NULL;
        HRESULT hr = CComObject<CDevice>::CreateInstance( &pMyDevice );
        if( SUCCEEDED (hr) )
        {
            pMyDevice->AddRef();
            hr = pMyDevice->QueryInterface( __uuidof(IUnknown),(void **) ppUnkwn);
            if (hr == S_OK)
            {
                pMyDevice->m_pWpdBaseDriver = pWpdBaseDriver;
            }
            pMyDevice->Release();
            pMyDevice = NULL;
        }

        return hr;
    }

    // IPnpCallback
    //
    STDMETHOD_(HRESULT, OnD0Entry)        ( /*[in]*/ IWDFDevice* pDevice,  /*[in] */WDF_POWER_DEVICE_STATE previousState);
    STDMETHOD_(HRESULT, OnD0Exit)         ( /*[in]*/ IWDFDevice* pDevice,  /*[in] */WDF_POWER_DEVICE_STATE newState);
    STDMETHOD_(VOID,     OnSurpriseRemoval)( /*[in]*/ IWDFDevice* pDevice);
    STDMETHOD_(HRESULT, OnQueryRemove)    ( /*[in]*/ IWDFDevice* pDevice);
    STDMETHOD_(HRESULT, OnQueryStop)      ( /*[in]*/ IWDFDevice* pDevice);

    // IPnpCallbackSelfManagedIo
    //
    STDMETHOD_(VOID,     OnSelfManagedIoCleanup)(/*[in]*/ IWDFDevice* pDevice);
    STDMETHOD_(VOID,     OnSelfManagedIoFlush)  (/*[in]*/ IWDFDevice* pDevice);
    STDMETHOD_(HRESULT, OnSelfManagedIoInit)   (/*[in]*/ IWDFDevice* pDevice);
    STDMETHOD_(HRESULT, OnSelfManagedIoSuspend)(/*[in]*/ IWDFDevice* pDevice);
    STDMETHOD_(HRESULT, OnSelfManagedIoRestart)(/*[in]*/ IWDFDevice* pDevice);
    STDMETHOD_(HRESULT, OnSelfManagedIoStop)   (/*[in]*/ IWDFDevice* pDevice);

    // IPnpCallbackHardware
    //
    STDMETHOD_(HRESULT, OnPrepareHardware)(/*[in]*/ IWDFDevice* pDevice);
    STDMETHOD_(HRESULT, OnReleaseHardware)(/*[in]*/ IWDFDevice* pDevice);

private:
    HRESULT GetSupportedContentTypes(
        IPortableDevicePropVariantCollection** ppContentTypes);

    HRESULT GetDeviceFriendlyName(
        __deref_out_opt LPWSTR*   pwszDeviceFriendlyName);

    HRESULT CreateIDValues(
        __in LPCWSTR pszManufacturer,
        __in LPCWSTR pszModel,
        __in_opt LPCWSTR pszVersion,
        __in GUID *pguidFUID,
        __out IPortableDeviceValues** ppValues
        );

private:

    WpdBaseDriver*                         m_pWpdBaseDriver;
    CComPtr<IPortableDeviceClassExtension> m_pPortableDeviceClassExtension;
};


